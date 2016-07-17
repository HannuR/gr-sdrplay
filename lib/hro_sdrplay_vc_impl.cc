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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "mirsdrapi-rsp.h"
#include "hro_sdrplay_vc_impl.h"

#define DEFAULT_SAMPLE_RATE		2048000
#define DEFAULT_BUF_LENGTH		(336 * 2) // (16 * 16384)
#define MINIMAL_BUF_LENGTH		672 // 512
#define MAXIMAL_BUF_LENGTH		(256 * 16384)


namespace gr {
  namespace sdrplay {

    hro_sdrplay_vc::sptr
    hro_sdrplay_vc::make(int gRdb, float fsMHz, float rfMHz, int bwType, int lfType)
    {
      return gnuradio::get_initial_sptr
        (new hro_sdrplay_vc_impl(gRdb, fsMHz, rfMHz, bwType, lfType));
    }

    /*
     * The private constructor
     */
    hro_sdrplay_vc_impl::hro_sdrplay_vc_impl(int gRdb, float fsMHz, float rfMHz, int bwType, int lfType)
      : gr::sync_block("hro_sdrplay_vc",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
      doWrk	= NULL;
      printf("*** Ctor. %i  %f %f  %i %i\n", gRdb, fsMHz, rfMHz, bwType, lfType );
      
      m_gRdb	= gRdb;
      m_fsMHz	= fsMHz;
      m_rfMHz	= rfMHz;
      m_bwType	= (mir_sdr_Bw_MHzT)bwType;
      m_lfType	= (mir_sdr_If_kHzT)lfType;
       
      printf("*** Ctor. %i  %f %f  %i %i\n", m_gRdb, m_fsMHz, m_rfMHz, m_bwType, m_lfType );
    }

    /*
     * Our virtual destructor.
     */
    hro_sdrplay_vc_impl::~hro_sdrplay_vc_impl()
    {
      // clean ups...
      if( doWrk != NULL  )	delete	doWrk;
      doWrk	= NULL;
    }

    int
    hro_sdrplay_vc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int  retVal;
      gr_complex *out = (gr_complex *) output_items[0];
      
      if( doWrk == NULL ) {
	//printf(".%i %i\n", 0, 0 );
	return 0;
      }
      rbuf	= doWrk->getRbuf();
      ibuf	= doWrk->getIbuf();
      //printf(".%i %i\n", noutput_items, doWrk->buffSize() );
      retVal	= doWrk->runit();
      
      // copy the data if ok, if not we had no data to return.
      if( retVal != 0 )
	return 0;
      
      // copy the data over...
      for( int ii=0; ii<doWrk->buffSize(); ii++ ) {
	out[ii]	= std::complex<float>( rbuf[ii], ibuf[ii] );
      }
      
      // Tell runtime system how many output items we produced. Not noutput_items
      return doWrk->buffSize();
      
    }

    // Called to enable drivers, etc for i/o devices.    
    bool hro_sdrplay_vc_impl::start() {
      printf("*** Start\n");
      
      doWrk = new HroCode::worker(m_gRdb,m_fsMHz,m_rfMHz,m_bwType,m_lfType);
      doWrk->config();

      return	true;
    }
    
    // Called to disable drivers, etc for i/o devices.
    bool hro_sdrplay_vc_impl::stop() {
      printf("*** Stop\n");
       
      int  re	= doWrk->cleanup();
      delete doWrk;
      doWrk	= NULL;

      return  re == mir_sdr_Success;
    }
    
  } /* namespace sdrplay */
} /* namespace gr */

