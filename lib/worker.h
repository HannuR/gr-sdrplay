#include <unistd.h>
#include "mirsdrapi-rsp.h"

namespace HroCode {

class worker {
		private:
			worker();

		public:
  	 	virtual ~worker();
		
		// configuration ctor...
		worker( int gRdB, double fsMHz, double rfMHz, int bwType, int ifType );

		int		gRdB;	// Gain reduction in dB, see default gain reduction tables, section 5 [0-102]
		double 	fsMHz;	// Specifies the ADC sample frequency in MHz, typical values between 0.5MHz and 12MHz
		double	rfMHz;	// Specifies the tuner centre frequency in MHz, see frequency allocation tables, section 6
		int	bwType;		// Specifies the bandwidth to be used, see list in enumerated type for supported modes.
						//  mir_sdr_Bw_MHzT;
		int	ifType;		// Specifies the IF to be used, see list in enumerated type for supported modes.
						//  mir_sdr_If_kHzT;

		// runtime methods...
  	 	virtual int config();
  	 	virtual int	runit();
  	 	virtual int	cleanup();
		
		// accessors
		short* getRbuf();
		short* getIbuf();
		int    buffSize();

  	 private:
  	 	bool	init_done;		// true if initialized succesfully, sdr init.
  	 	int	samplesPerPacket;	// init defined
  	 	short*	rbuf;
  	 	short*	ibuf;

		unsigned int firstSampleNum;	// sample num, from prev mir_sdr_ReadPacket

};

}  // namespace HroCode
