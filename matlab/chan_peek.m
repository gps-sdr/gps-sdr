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

% float header;
% float chan;
% float sv;
% float bit_lock;
% float frame_lock;
% float subframe;
% float best_epoch;
% float count;
% float len;	
% float I[0];
% float Q[0];
% float I[1];
% float Q[1];
% float I[2];
% float Q[2];
% float I_avg;
% float Q_var;
% float P_avg;
% float CN0;
% float CN0_old;
% float code_nco;
% float carrier_nco;
% float fll_lock;
% float pll_lock;
% float fll_lock_ticks;
% float w;
% float x;
% float z;

power = A(:,10:12).^2 + A(:,13:15).^2;

figure(1)

subplot(311)
plot(A(:,9)); ylabel('Integration Length');
grid on;

subplot(312)
plot(power)
grid on;
plot(A(:,18).*A(:,9),'k')
ylabel('I^{2}+Q^{2}');
grid on;
subplot(313)
plot(A(:,21) - 1.023e6) 
grid on; hold on;
plot((A(:,22)-604000)*1.023e6/1.57542e9,'r')
ylabel('Code NCO');

figure(2)
subplot(311)
plot(A(:,26)); ylabel('Acceleration');
hold on; grid on;
subplot(312)
plot(A(:,27)); ylabel('Velocity');
hold on; grid on;
subplot(313)
plot(A(:,28)); ylabel('Carrier NCO');
hold on; grid on;

figure(3)

subplot(211)
plot(A(:,23),'b')
hold on; grid on;
ylabel('FLL Lock')

subplot(212)
plot(A(:,24),'r')
hold on; grid on;
ylabel('PLL Lock')


figure(4)
subplot(211)
plot(A(:,11),'b.')
hold on; grid on;
plot(A(:,14),'r.')
legend('Inphase','Quadrature')
ylabel('Correlation')
subplot(212)
plot(A(:,19))
hold on; grid on;
plot(A(:,20),'r')
ylabel('CN_{0}')

figure(5)
plot(A(:,11) + i*A(:,14),'b.')

figure(6)

subplot(211)
plot(A(:,7),'b')
hold on; grid on;
ylabel('Best Epoch')