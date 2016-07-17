#include <iostream>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include "worker.h"
using namespace std;

namespace HroCode {

worker::worker() {
	init_done	= false;
	rbuf	= NULL;
	ibuf	= NULL;
}

worker::worker( int gRdB, double fsMHz, double rfMHz, int bwType, int ifType ) {
	worker();
  	this->gRdB 	= gRdB;
  	this->fsMHz	= fsMHz;
  	this->rfMHz	= rfMHz;
  	this->bwType	= bwType;
  	this->ifType	= ifType;
}
  

worker::~worker() {
	cleanup();
}

short* worker::getRbuf() {
  return rbuf;
}

short* worker::getIbuf() {
  return ibuf;
}

int worker::buffSize() {
  return samplesPerPacket;
}


int worker::config() {
	mir_sdr_ErrT  ret	= mir_sdr_Success;
	mir_sdr_ErrT  dcRet1, dcRet2;

	printf("### config \n");

/*
	Parameters:
	gRdB	Gain reduction in dB, see default gain reduction tables, section 5 [0-102]
	fsMHz	Specifies the ADC sample frequency in MHz, typical values between 0.5MHz and 12MHz
	rfMHz	Specifies the tuner centre frequency in MHz, see frequency allocation tables, section 6
	bwType	Specifies the bandwidth to be used, see list in enumerated type for supported modes.
	ifType	Specifies the IF to be used, see list in enumerated type for supported modes.
	samplesPerPacket
			Pointer to an unsigned integer which returns the number of samples that will be returned
			for this configuration in each call to mir_sdr_ReadPacket()

	mir_sdr_BW_0_200 = 200,
	mir_sdr_BW_0_300 = 300,
	mir_sdr_BW_0_600 = 600,
	mir_sdr_BW_1_536 = 1536,
	mir_sdr_BW_5_000 = 5000,
	mir_sdr_BW_6_000 = 6000,
	mir_sdr_BW_7_000 = 7000,
	mir_sdr_BW_8_000 = 8000

	mir_sdr_IF_Zero   = 0,
	mir_sdr_IF_0_450  = 450,
	mir_sdr_IF_1_620  = 1620,
	mir_sdr_IF_2_048  = 2048
*/

/*
	optional LNA, not used.
    mir_sdr_SetParam(201,1);
    //mir_sdr_SetParam(202,1);
*/

	ret	= mir_sdr_Init( gRdB, fsMHz, rfMHz, (mir_sdr_Bw_MHzT)bwType, (mir_sdr_If_kHzT)ifType, &samplesPerPacket);
	if(ret != mir_sdr_Success)
		return	ret;

	// setup dc correction...
	dcRet1	= mir_sdr_SetDcMode(4,0);
	dcRet2	= mir_sdr_SetDcTrackTime(63);

    printf("### DC Ret %i %i\n", dcRet1, dcRet2 );

	// Ok, mark we must uninit...
	init_done	= true;

	// Allocate buffers...
	int	size	= samplesPerPacket*sizeof(short);
	rbuf	= (short*)malloc(size);
	ibuf	= (short*)malloc(size);

	return	ret;
}

int	worker::runit() {
	mir_sdr_ErrT  ret	= mir_sdr_Success;
	if( !init_done ) {
		return	ret;
	}

	unsigned int fiSampNum;
	int	grChanged, rfChanged, fsChanged;
	ret	= mir_sdr_ReadPacket( ibuf, rbuf, &fiSampNum, &grChanged, &rfChanged, &fsChanged );

	firstSampleNum	= fiSampNum;

	return	ret;
}


int	worker::cleanup() {
	mir_sdr_ErrT  ret	= mir_sdr_Fail;
	printf("### cleanup \n");

	if( init_done ) {
		ret	= mir_sdr_Uninit();
	}

	if( rbuf != NULL )	free(rbuf);
	if( ibuf != NULL )	free(ibuf);

	init_done	= false;
	rbuf		= NULL;
	ibuf		= NULL;

	return	ret;

}

}  // namespace HroCode