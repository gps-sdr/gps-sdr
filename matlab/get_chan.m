function [A] = get_chan(chan)

% uint32 tic;			//!< Corresponds to this receiver tic
% int32 chan;			//!< The channel number
% int32 sv;			//!< SV/PRN number the channel is tracking
% int32 state;		//!< channel's state
% int32 antenna;		//!< Antenna channel is tracking off of
% int32 len;			//!< acummulation length (1 or 20 ms)
% int32 w;			//!< 3rd order PLL state
% int32 x;			//!< 3rd order PLL state
% int32 z;			//!< 3rd order PLL state
% int32 code_nco;		//!< State of code_nco
% int32 carrier_nco;	//!< State of carrier_nco
% int32 cn0;			//!< CN0 estimate
% int32 p_avg;		//!< Filtered version of I^2+Q^2
% int32 bit_lock;		//!< Bit lock?
% int32 frame_lock;	//!< Frame lock?
% int32 navigate;		//!< Navigate on this channel flag
% int32 count;		//!< Number of accumulations that have been processed
% int32 subframe;		//!< Current subframe number
% int32 best_epoch;	//!< Best estimate of bit edge position


pts = 19;

fp = fopen(sprintf('../chan%02d.dat',chan),'rb');
A(:,1) = fread(fp,inf,'int32'); 

len = floor(length(A)/pts);
A = A(1:len*pts);

A = reshape(A, [pts len]).';