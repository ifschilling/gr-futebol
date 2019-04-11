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

#ifndef INCLUDED_FUTEBOL_MSG_RSSI_CSV_IMPL_H
#define INCLUDED_FUTEBOL_MSG_RSSI_CSV_IMPL_H

#include <futebol/msg_rssi_csv.h>

namespace gr {
  namespace futebol {

    class msg_rssi_csv_impl : public msg_rssi_csv
    {
     private:
      void handle_pdu(pmt::pmt_t pdu);

      float d_rssi;
      int d_msg_offset;
			int d_msg_len;
      char* d_msg;
      float d_threshold;

     public:
      msg_rssi_csv_impl(float threshold);
      ~msg_rssi_csv_impl();

      // Where all the action really happens
      //void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

    struct csv_hdr {
      char sender[7]; // 0x.... \0
      char sec[7]; //xxxxxx. \0
      char usec[7]; // 0.xxxxxx \0
	  }__attribute__((packed));


  } // namespace futebol
} // namespace gr

#endif /* INCLUDED_FUTEBOL_MSG_RSSI_CSV_IMPL_H */

