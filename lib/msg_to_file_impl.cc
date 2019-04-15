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
#include "msg_to_file_impl.h"

using namespace gr::futebol;
class msg_to_file_impl : public msg_to_file{ 
public:
  /*
    * The private constructor
    */
  msg_to_file_impl():
    block("msg_to_file",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(1, 1, sizeof(uint8_t))),
          d_msg_offset(0)
  {
    message_port_register_in(pmt::mp("in"));
    d_msg_len = 0;
    d_msg = NULL;
  }

  /*
    * Our virtual destructor.
    */
  ~msg_to_file_impl()
  {
  }

  int
  general_work (int noutput_items,
                      gr_vector_int &ninput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
  {
    gr_complex *out = (gr_complex*)output_items[0];

    if(!d_msg_len){
      pmt::pmt_t msg(delete_head_blocking(pmt::mp("in")));
      if(!msg) {
        return 0;
      }

      if(pmt::is_eof_object(msg)) {
        //dout << "WIRESHARK: exiting" << std::endl;
        return -1;
      } else if(pmt::is_pair(msg)) {
        //dout << "WIRESHARK: received new message" << std::endl;
        //dout << "message length " << pmt::blob_length(pmt::cdr(msg)) << std::endl;
        handle_pdu(msg);
      } else {
        //dout << "WIRESHARK: ignoring message" << std::endl;
        return 0;
      }
    }

    int to_copy = std::min((d_msg_len - d_msg_offset), noutput_items);
    memcpy(out, d_msg + d_msg_offset, to_copy);

    /*
    dout << "WIRESHARK: d_msg_offset: " <<  d_msg_offset <<
      "   to_copy: " << to_copy << 
      "   d_msg_len " << d_msg_len << std::endl;
    */

    d_msg_offset += to_copy;

    if(d_msg_offset == d_msg_len) {
      d_msg_offset = 0;
      d_msg_len = 0;
      std::free(d_msg);
    }
    /*
    dout << "WIRESHARK: output size: " <<  noutput <<
      "   produced items: " << to_copy << std::endl;
    */
    return to_copy;
  }

private:
  int d_msg_offset;
  int d_msg_len;
  char* d_msg;
  void
  handle_pdu(pmt::pmt_t pdu) {
    const char *buf = reinterpret_cast<const char*>(pmt::blob_data(pmt::cdr(pdu)));
    d_msg_len = pmt::blob_length(pmt::cdr(pdu));

    d_msg = reinterpret_cast<char*>(std::malloc(d_msg_len));

    memcpy(d_msg, buf, d_msg_len);
  }
};

msg_to_file::sptr
msg_to_file::make()
{
  return gnuradio::get_initial_sptr(new msg_to_file_impl());
}