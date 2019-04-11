/* -*- c++ -*- */

#define FUTEBOL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "futebol_swig_doc.i"

%{
#include "futebol/msg_rssi_csv.h"
#include "futebol/msg_rrsi_msg.h"
%}


%include "futebol/msg_rssi_csv.h"
GR_SWIG_BLOCK_MAGIC2(futebol, msg_rssi_csv);
%include "futebol/msg_rrsi_msg.h"
GR_SWIG_BLOCK_MAGIC2(futebol, msg_rrsi_msg);
