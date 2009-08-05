% CHAN_PEEK Inspect the output from the a given tracking channel
%   CHAN_PEEK(chan) is used to visualize the status of a given tracking 
%   channel. Figures include:
%   1: Code Tracking
%   2: Carrier Tracking
%   3: PLL and FLL lock indicators
%   4: Raw Correlations & CN0

function [A] = chan_peek(chan)

A = get_chan(chan);

% uint32 tic;		//!< Corresponds to this receiver tic
% int32 chan;		//!< The channel number
% int32 sv;			//!< SV/PRN number the channel is tracking
% int32 state;		//!< channel's state
% int32 antenna;	//!< Antenna channel is tracking off of
% int32 len;		//!< acummulation length (1 or 20 ms)
% int32 w;			//!< 3rd order PLL state
% int32 x;			//!< 3rd order PLL state
% int32 z;			//!< 3rd order PLL state
% int32 code_nco;	//!< State of code_nco
% int32 carrier_nco;//!< State of carrier_nco
% int32 cn0;		//!< CN0 estimate
% int32 p_avg;		//!< Filtered version of I^2+Q^2
% int32 bit_lock;	//!< Bit lock?
% int32 frame_lock;	//!< Frame lock?
% int32 navigate;	//!< Navigate on this channel flag
% int32 count;		//!< Number of accumulations that have been processed
% int32 subframe;	//!< Current subframe number
% int32 best_epoch;	//!< Best estimate of bit edge position
% I[0]
% Q[0]
% P_buff[0]


figure
hold all; grid on;
plot((A(:,21).^2+A(:,24).^2),'.')
plot((A(:,22).^2+A(:,25).^2),'r.')
plot(A(:,13)./(A(:,6)),'k')

figure
cor = A(:,21)+i*A(:,24);
plot(real(cor),'b.')

figure
hold all; grid on;
plot(A(:,7))
plot(A(:,9))
plot(A(:,8)*0.5)

% bl = A(:,end-19:end);
% 
% figure
% mesh(bl)
% shading interp;


