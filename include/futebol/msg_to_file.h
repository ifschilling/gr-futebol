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


#ifndef INCLUDED_FUTEBOL_MSG_TO_FILE_H
#define INCLUDED_FUTEBOL_MSG_TO_FILE_H

#include <futebol/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace futebol {

    /*!
     * \brief <+description of block+>
     * \ingroup futebol
     *
     */
    class FUTEBOL_API msg_to_file : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<msg_to_file> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of futebol::msg_to_file.
       *
       * To avoid accidental use of raw pointers, futebol::msg_to_file's
       * constructor is in a private implementation
       * class. futebol::msg_to_file::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace futebol
} // namespace gr

#endif /* INCLUDED_FUTEBOL_MSG_TO_FILE_H */

