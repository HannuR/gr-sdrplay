/* -*- c++ -*- */

#define SDRPLAY_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sdrplay_swig_doc.i"

%{
#include "sdrplay/hro_sdrplay_vc.h"
%}


%include "sdrplay/hro_sdrplay_vc.h"
GR_SWIG_BLOCK_MAGIC2(sdrplay, hro_sdrplay_vc);
