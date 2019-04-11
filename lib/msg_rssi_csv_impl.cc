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
#include "msg_rssi_csv_impl.h"

#include <sys/time.h>

std::string string_to_hex(const std::string& input)
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

namespace gr {
  namespace futebol {
    float sum_rssi = 0;
	  int count = 0;

    msg_rssi_csv::sptr
    msg_rssi_csv::make(float threshold)
    {
      return gnuradio::get_initial_sptr
        (new msg_rssi_csv_impl(threshold));
    }

    /*
     * The private constructor
     */
    msg_rssi_csv_impl::msg_rssi_csv_impl(float threshold)
      : gr::block("msg_rssi_csv",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(uint8_t))),
              d_msg_offset(0),
              d_threshold(threshold)
    {
      message_port_register_in(pmt::mp("msg_in"));
      d_msg_len = 0;
      d_msg = NULL;
    }

    /*
     * Our virtual destructor.
     */
    msg_rssi_csv_impl::~msg_rssi_csv_impl()
    {
    }

    /*void
    msg_rssi_csv_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      <+forecast+> e.g. ninput_items_required[0] = noutput_items
    }*/

    void
    msg_rssi_csv_impl::handle_pdu(pmt::pmt_t pdu){
      // get current time
      struct timeval t;
      gettimeofday(&t, NULL);

      std::string str;
      str = pmt::serialize_str(pmt::cdr(pdu));
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

      std::size_t len = data.length();
      std::size_t offset = 0;

      d_msg = reinterpret_cast<char*>(std::malloc(len + sizeof(csv_hdr)));
      csv_hdr *hdr = reinterpret_cast<csv_hdr*>(d_msg);
      strcpy(hdr->sender, source.c_str());

      sprintf(hdr->sec, "%.6d", (int) t.tv_sec);
      sprintf(hdr->usec, "%.6d", (int) t.tv_usec);
      offset += sizeof(csv_hdr);

      memcpy(d_msg + offset, data.c_str(), len);
	    d_msg_len = offset + len;
    }

    int
    msg_rssi_csv_impl::general_work (int noutput_items,
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

     /*pmt::pmt_t msg(delete_head_blocking(pmt::mp("msg_in")));

      if(!d_msg_len) {
        if(msg && pmt::is_pair(msg)) {
          //dout << "WIRESHARK: received new message" << std::endl;
          //dout << "message length " << pmt::blob_length(pmt::cdr(msg)) << std::endl;
          handle_pdu(msg);
          int to_copy = std::min((d_msg_len - d_msg_offset), noutput_items);
          memcpy(out, d_msg + d_msg_offset, to_copy);

          d_msg_offset += to_copy;

          if(d_msg_offset == d_msg_len) {
            d_msg_offset = 0;
            d_msg_len = 0;
            std::free(d_msg);
          }
        }
      }
      if(d_msg_len){
        int to_copy = std::min((d_msg_len - d_msg_offset), noutput_items);
        memcpy(out, d_msg + d_msg_offset, to_copy);

        d_msg_offset += to_copy;

        if(d_msg_offset == d_msg_len) {
          d_msg_offset = 0;
          d_msg_len = 0;
          std::free(d_msg);
        }
      }
      else
      {
        out=NULL;
      }*/
      
      //consume the inputs
      std::cout << "ok!!!" << std::endl;
	    this->consume(0, noutput_items); //consume port 0 input

	    this->consume_each(noutput_items); //or shortcut to consume on all inputs
      /*dout << "WIRESHARK: output size: " <<  noutput_items <<
      "   produced items: " << to_copy << std::endl;*/
      return noutput_items;
    }

  } /* namespace futebol */
} /* namespace gr */

