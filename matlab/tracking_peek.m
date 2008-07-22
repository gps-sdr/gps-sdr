% TRACKING_PEEK Inspect the output from the tracking channels
%   TRACKING_PEEK() is used to inspect the tracking status. Figures include:
%   1: SV Tracking Status
%   2: CN) Estimates

close all;
clear; clc;

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

%path = '/home/gheckler/GPS_Data/Car_Test3/'
path = '/home/gwheckler/workspace/gps-sdr/';
fname = [path,'tracking.tlm'];

a = dlmread(fname);
len = floor(length(a)/12);
a = a(1:12*len,:);
dt = [1:len]/600;

svs         = reshape(a(:,2),[12 len]).';
bit_lock    = reshape(a(:,4),[12 len]).';
frame_lock  = reshape(a(:,5),[12 len]).';
P_avg       = reshape(a(:,8),[12 len]).';
CN0         = reshape(a(:,9),[12 len]).';
fll         = reshape(a(:,10),[12 len]).';
pll         = reshape(a(:,11),[12 len]).';
fll_lock    = reshape(a(:,12),[12 len]).';

mask = or((~bit_lock), (CN0 < 20));

svs(mask) = NaN;
CN0(mask) = NaN;
pll(mask) = NaN;
fll(mask) = NaN;

b = CN0(:);
a = svs(:);

for(lcv = 1:32)
    
    [indx, indy] = find(svs == lcv);
    ind = find(a == lcv);
    if(indx)
        CN0sv(lcv,indx) = b(ind);
    end
    
end

CN0sv(CN0sv == 0) = NaN;

figure
plot(dt,svs,'LineWidth',5)
grid on; axis([min(dt) max(dt) 0 33])
xlabel('Time (minutes)')
ylabel('SV Tracked')
title('SV Plot')
print -dpng -r0 sv.png

figure
plot(dt,CN0sv)
grid on; axis([min(dt) max(dt) 25 50])
xlabel('Time (minutes)')
ylabel('C/N_{0} Estimate (dB-Hz)')
title('C/N_{0} Plot')
print -dpng -r0 cn0.png

figure
plot(dt,pll)
xlabel('Time (minutes)')
