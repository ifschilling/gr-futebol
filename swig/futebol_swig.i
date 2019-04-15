/* -*- c++ -*- */

#define FUTEBOL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "futebol_swig_doc.i"

%{
#include "futebol/msg_rssi_msg.h"
#include "futebol/msg_to_file.h"
%}

%include "futebol/msg_rssi_msg.h"
GR_SWIG_BLOCK_MAGIC2(futebol, msg_rssi_msg);
%include "futebol/msg_to_file.h"
GR_SWIG_BLOCK_MAGIC2(futebol, msg_to_file);
