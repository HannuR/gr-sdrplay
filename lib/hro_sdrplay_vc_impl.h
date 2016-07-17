/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_SDRPLAY_HRO_SDRPLAY_VC_IMPL_H
#define INCLUDED_SDRPLAY_HRO_SDRPLAY_VC_IMPL_H

#include <sdrplay/hro_sdrplay_vc.h>
#include "worker.h"

namespace gr {
  namespace sdrplay {

    class hro_sdrplay_vc_impl : public hro_sdrplay_vc
    {
     private:
       // short buffer	// array to read in nsample * sizeos short
       short* rbuf;
       short* ibuf;
       HroCode::worker*	doWrk;
       
       // Parameters:
       int   m_gRdb;	// Gain reduction in dB, see default gain reduction tables, section 5
       float m_fsMHz;	// Specifies the ADC sample frequency in MHz, typical values between 0.5MHz and 12MHz
       float m_rfMHz;	// Specifies the tuner centre frequency in MHz, see frequency allocation tables, section 6
       mir_sdr_Bw_MHzT
	     m_bwType;	// Specifies the bandwidth to be used, see list in enumerated type for supported modes.
       mir_sdr_If_kHzT
	     m_lfType;	// Specifies the IF to be used, see list in enumerated type for supported modes.
       int samplesPerPacket;
			// Pointer to an unsigned integer which returns the number of samples that will be returned
			// for this configuration in each call to mir_sdr_ReadPacket()
       
      // Nothing to declare in this block.

     public:
      hro_sdrplay_vc_impl(int gRdb, float fsMHz, float rfMHz, int bwType, int lfType);
      ~hro_sdrplay_vc_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);

      // Called to enable drivers, etc for i/o devices.    
      virtual bool start();
      
      // Called to disable drivers, etc for i/o devices.
      virtual bool stop();
      
    };

  } // namespace sdrplay
} // namespace gr

#endif /* INCLUDED_SDRPLAY_HRO_SDRPLAY_VC_IMPL_H */

