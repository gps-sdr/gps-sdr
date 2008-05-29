% SATELLITE_PEEK Generate a skyplot
%   SATELLITE_PEEK() is used to generate a sky plot
%   1: Sky plot

close('all','force'); clear; clc;
pause(1.0);

% lcv,
% (int32)pChan->sv,
% pSV->time,
% pSV->x,
% pSV->y,
% pSV->z,
% pSV->vx,
% pSV->vy,
% pSV->vz
% pSV->elev
% pSV->azim

c = 2.99792458e8;

fname = sprintf('../satellites.tlm');
a = dlmread(fname);
len = floor(length(a)/12);
a = a(1:12*len,:);

svx = reshape(a(:,4),[12 len]).';
svy = reshape(a(:,5),[12 len]).';
svz = reshape(a(:,6),[12 len]).';
elev = reshape(a(:,10),[12 len]).';
azim = reshape(a(:,11),[12 len]).';

azim(find(azim == 0)) = NaN;
elev(find(elev == 0)) = NaN;
elev(find(elev < 0)) = NaN;
elev(find(elev > pi/2)) = NaN;

elev = 180*(pi-elev)/pi;
azim = azim + pi;

figure(4)
polar(azim,elev,'.');


