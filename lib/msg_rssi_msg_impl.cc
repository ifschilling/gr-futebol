/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "msg_rssi_msg_impl.h"

#include <sys/time.h>
#include <string> 

namespace gr {
	namespace futebol {

		msg_rssi_msg::sptr
		msg_rssi_msg::make(float threshold)
		{
			return gnuradio::get_initial_sptr
				(new msg_rssi_msg_impl(threshold));
		}

		msg_rssi_msg_impl::msg_rssi_msg_impl(float threshold)
			: gr::block("msg_rssi_msg",
					gr::io_signature::make(1, 1, sizeof(float)),
					gr::io_signature::make(0,0,0)),
			d_msg_offset(0),
			d_threshold(threshold)
		{
			message_port_register_in(pmt::mp("msg_in"));
			set_msg_handler(pmt::mp("msg_in"), boost::bind(&msg_rssi_msg_impl::msg_in, this, _1));
			message_port_register_out(pmt::mp("msg_out"));
			d_msg_len = 0;
			d_msg = NULL;
			sum_rssi = 0;
			count = 0;
			gettimeofday(&t_start, NULL);
		}

		/*
		* Our virtual destructor.
		*/
		msg_rssi_msg_impl::~msg_rssi_msg_impl()
		{
		}


		uint16_t
		msg_rssi_msg_impl::crc16(char *buf, int len)
		{
			uint16_t crc = 0;

			for(int i = 0; i < len; i++) {
				for(int k = 0; k < 8; k++) {
					int input_bit = (!!(buf[i] & (1 << k)) ^ (crc & 1));
					crc = crc >> 1;
					if(input_bit) {
						crc ^= (1 << 15);
						crc ^= (1 << 10);
						crc ^= (1 <<  3);
					}
				}
			}

			return crc;
		}

		int
			msg_rssi_msg_impl::general_work (int noutput_items,
					gr_vector_int &ninput_items,
					gr_vector_const_void_star &input_items,
					gr_vector_void_star &output_items)
			{
				const float *in = (const float *) input_items[0];
				gr_complex *out = (gr_complex *) output_items[0];

				for(size_t i = 0; i < noutput_items; i++) {
					if((in[i] > d_threshold) && (in[i] < -0.0001)){
						sum_rssi += in[i];
						count++;
					}
				}

				//consume the inputs
				this->consume(0, noutput_items); //consume port 0 input

				this->consume_each(noutput_items); //or shortcut to consume on all inputs

				return noutput_items;
			}

		void
		msg_rssi_msg_impl::msg_in(pmt::pmt_t msg)
		{
			// get current time
			struct timeval t_aux;
			gettimeofday(&t_aux, NULL);

			pmt::pmt_t blob;
			if(pmt::is_pair(msg)){
				blob = pmt::cdr(msg);
			}
			else{
				sum_rssi = 0;
				count = 0;
				return;
			}

			size_t data_len = pmt::blob_length(blob);
			if(data_len < 11) {
				//dout << "MAC: frame too short. Dropping!" << std::endl;
				sum_rssi = 0;
				count = 0;
				return;
			}

			uint16_t crc = crc16((char*)pmt::blob_data(blob), data_len);
			if(crc) {
				//dout << "MAC: wrong crc. Dropping packet!" << std::endl;
				sum_rssi = 0;
				count = 0;
				return;
			}

			std::string str;
			str = pmt::serialize_str(blob);
			str = str.substr(8);
			/*
				0,1 = Frame Control Field
				2 = Sequence Number
				3,4 = Destination PAN
				5,6 = Destination
				7,8 = Source
				9 - 12 = ?
				13 - (n-3) = Data
				(n-2),(n-1) = FCS
				*/
			std::string data = str.substr(13,(str.size()-3-13));
			std::string source = "0x";
			source.append(string_to_hex(str.substr(8,1)));
			source.append(string_to_hex(str.substr(7,1)));
			//dout << "DATA: " << data << std::endl;
			//dout << "Source: " << source << std::endl;
			//dout << "BLOB: " << string_to_hex(str) << std::endl;
			//dout << "MAC: correct crc. Propagate packet to APP layer. RSSI: " << (sum_rssi/count) << " Samples: " << count << std::endl;


			// buf = source,t_sec,t_usec,rssi,"data"\n
			std::string buf;
			buf = source;
			buf.append(",");
			struct timeval t;
			timersub(&t_aux, &t_start, &t);
			buf.append(std::to_string((int)t.tv_sec));
			buf.append(",");
			buf.append(std::to_string((int)t.tv_usec));
			buf.append(",");
			buf.append(std::to_string((float)(sum_rssi/count)));
			buf.append(",\"");
			buf.append(data);
			buf.append("\"\n");

			sum_rssi = 0;
			count = 0;

			pmt::pmt_t payload = pmt::make_blob((char*)buf.c_str(), buf.length()+1);

			message_port_pub(pmt::mp("msg_out"), pmt::cons(pmt::PMT_NIL, payload));
		}


		std::string
		msg_rssi_msg_impl::string_to_hex(const std::string& input)
		{
			static const char* const lut = "0123456789ABCDEF";
			size_t len = input.length();

			std::string output;
			output.reserve(2 * len);
			for (size_t i = 0; i < len; ++i)
			{
					const unsigned char c = input[i];
					output.push_back(lut[c >> 4]);
					output.push_back(lut[c & 15]);
			}
			return output;
		}

	}
}
