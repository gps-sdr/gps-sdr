% SATELLITE_PEEK Generate a skyplot
%   SATELLITE_PEEK() is used to generate a sky plot
%   1: Sky plot

close all; clear; clc;

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
if(len ~= length(a))
    a = a(1:12*len,:);
end

elev = reshape(a(:,10),[12 len]).';
azim = reshape(a(:,11),[12 len]).';

azim(find(azim == 0)) = NaN;
elev(find(elev == 0)) = NaN;
mask = find(elev < 0);
elev(mask) = -elev(mask);


% Create the circles
th = linspace(0,2*pi,50);
th = [th, th(1)];
xunit = cos(th.'); yunit = sin(th.');

% Plot the spokes
th = linspace(0,2*pi,13);
th = th(2:end);
cst = cos(th); snt = sin(th);
cs = [-cst; cst];
sn = [-snt; snt];

h = figure;
set(h,'Color','w')
hold on;

% Spokes
plot(cs,sn,'k')

% 15 degree circles
rticks = 0:30:90;
rticks = rticks/90;
plot(xunit*rticks,yunit*rticks,'k')
for(i = 1:length(rticks)-1)
    text(rticks(i)+.05,.01,sprintf('%02d',90-rticks(i)*90))
end

% Scale elevation
elev = 1-elev/(pi/2);

% Rotate azim to have north be up
azim = -azim + pi/2;

% Transform to cartesian
xx = elev.*cos(azim);
yy = elev.*sin(azim);

% Plot it
plot(xx,yy,'.','LineWidth',4)

axis equal; axis off;

text(1.05,0,'East')
text(-.1,1.1,'North')
text(-1.2,0,'West')
text(-.1,-1.1,'South')

print -dpng -r0 skyplot