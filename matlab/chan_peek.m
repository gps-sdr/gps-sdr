% CHAN_PEEK Inspect the output from the a given tracking channel
%   CHAN_PEEK(chan) is used to visualize the status of a given tracking 
%   channel. Figures include:
%   1: Code Tracking
%   2: Carrier Tracking
%   3: PLL and FLL lock indicators
%   4: Raw Correlations & CN0

function [] = chan_peek(chan)

close all; clc;
A = get_chan(chan);

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



figure(1)

subplot(311)
plot(A(:,3)); ylabel('Integration Length');
grid on;

subplot(312)
plot(A(:,13),'k')
hold on; grid on;
ylabel('I^{2}+Q^{2}');

return

subplot(313)
plot(A(:,21)-1.023e6) 
hold on;
plot(A(:,22)*1.023e6/1.57542e9,'r')
hold off; grid on;
ylabel('Code NCO');

figure(2)
subplot(311)
plot(A(:,26)); ylabel('Acceleration');
grid on;

subplot(312)
plot(A(:,27)); ylabel('Velocity');
grid on;

subplot(313)
plot(A(:,28)); ylabel('Carrier NCO');
grid on;

figure(3)

subplot(211)
plot(A(:,23),'b')
grid on;
ylabel('FLL Lock')

subplot(212)
plot(A(:,24),'r')
grid on;
ylabel('PLL Lock')


figure(4)
subplot(211)
plot(A(:,11),'b.')
hold on;
plot(A(:,14),'r.')
hold off; grid on;
legend('Inphase','Quadrature')
ylabel('Correlation')

subplot(212)
plot(A(:,19))
hold on;
plot(A(:,20),'r')
hold off; grid on;
ylabel('CN_{0}')
