% TRACKING_PEEK Inspect the output from the tracking channels
%   TRACKING_PEEK() is used to inspect the tracking status. Figures include:
%   1: SV Tracking Status
%   2: CN) Estimates

close all; clear; clc;

% lcv,
% (int32)pChan->sv,
% (int32)pChan->count,
% (int32)pChan->bit_lock,		
% (int32)pChan->frame_lock,		
% (int32)pChan->subframe,		
% (int32)pChan->len,
% pChan->P_avg,		
% pChan->CN0,		
% pChan->fll_lock,		
% pChan->pll_lock,		
% pChan->fll_lock_ticks);				
        
c = 2.99792458e8;

fname = sprintf('../tracking.tlm');
a = dlmread(fname);
len = floor(length(a)/12);
a = a(1:12*len,:);
dt = [1:len]/60;

svs         = reshape(a(:,2),[12 len]).';
bit_lock    = reshape(a(:,4),[12 len]).';
frame_lock  = reshape(a(:,5),[12 len]).';
P_avg       = reshape(a(:,8),[12 len]).';
CN0         = reshape(a(:,9),[12 len]).';
fll         = reshape(a(:,10),[12 len]).';
pll         = reshape(a(:,11),[12 len]).';
fll_lock    = reshape(a(:,12),[12 len]).';

mask = ~bit_lock;
%mask = 1:length(svs);

svs(mask) = NaN;
CN0(mask) = NaN;
w(mask) = NaN;
x(mask) = NaN;
z(mask) = NaN;

figure
plot(dt,svs,'LineWidth',5)
grid on; axis([min(dt) max(dt) 0 33])
xlabel('Time (minutes)')
ylabel('SV Tracked')
title('SV Plot')
print -dpng -r0 sv.png

figure
plot(dt,CN0)
grid on; axis([min(dt) max(dt) 20 55])
xlabel('Time (minutes)')
ylabel('C/N_{0} Estimate (dB-Hz)')
title('C/N_{0} Plot')
print -dpng -r0 cn0.png

% figure
% plot(dt,P_avg)
% grid on;
% xlabel('Time (minutes)')
% ylabel('I^2+Q^2 Estimate (dB-Hz)')
% title('Power Plot')

% figure
% subplot(211)
% plot(dt,fll)
% grid on;
% subplot(212)
% plot(dt,pll)
% grid on;

