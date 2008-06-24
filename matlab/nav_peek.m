% NAV_PEEK Inspect the output PVT solution
%   NAV_PEEK() is used to inspect the PVT output. Figures include:
%   1: Converged flag, 3 of SVs in PVt solution
%   2: Deviation from mean ECEF X, Y, and Z position
%   3: Deviation from mean ECEF X, Y, and Z velocity
%   4: Clock bias (seconds) and clock rate (meters/sec)
%   5: DOPs

close all; clear; clc;

c = 2.99792458e8;

path = '/home/gheckler/GPS_Data/Car_Test3/'
fname = [path,'navigation.tlm'];

a = dlmread(fname);
a = a(1:end-20,:);
ind = find(a(:,1) == 1);


% tx = 6378155.0;
tx = 0.0;
ty = 0.0;
tz = 0.0;

% tx = mean(a(ind,4));
% ty = mean(a(ind,5));
% tz = mean(a(ind,6));

% a(:,4) = a(:,4) - tx;
% a(:,5) = a(:,5) - ty;
% a(:,6) = a(:,6) - tz;

% a = a(ind,:);

len = length(a);
dt = [1:len]/60;

% pNav->converged,
% pNav->nsvs,
% pNav->tic,
% pNav->x,
% pNav->y,
% pNav->z,
% pNav->vx,
% pNav->vy,
% pNav->vz,
% pNav->clock_bias,
% pNav->clock_rate,
% pNav->gps_time,
% pNav->gdop,
% pNav->hdop,
% pNav->tdop,
% pNav->vdop,
% pNav->pdop);

figure
subplot(211)
plot(dt,a(:,1))
grid on; ylabel('Converged'); axis([0 max(dt) 0 2])
title('Nav Status')
subplot(212)
plot(dt,a(:,2))
grid on; ylabel('Nsvs'); axis([0 max(dt) 0 13])
xlabel('Minutes')
print -dpng -r0 nav_status

figure
subplot(311)
plot(dt,a(:,4))
grid on; ylabel('p_{x} (m)'); axis tight;
title('ECEF Position Deviation')
subplot(312)
plot(dt,a(:,5))
grid on; ylabel('p_{y} (m)'); axis tight;
subplot(313)
plot(dt,a(:,6))
grid on; ylabel('p_{z} (m)'); axis tight;
xlabel('Minutes')
% print -dpng -r0 postion

% 
% figure
% plot3(a(:,4),a(:,5),a(:,6),'.')
% axis tight; hold on; grid on;


figure
subplot(311)
plot(dt,a(:,7))
grid on; ylabel('v_{x} (m/s)'); axis tight;
title('ECEF Velocity Deviation')
subplot(312)
plot(dt,a(:,8))
grid on; ylabel('v_{y} (m/s)'); axis tight;
subplot(313)
% plot(dt,a(:,9))
plot(dt,sqrt(sum(a(:,7:9).^2,2))*2.2369)
grid on; ylabel('v_{z} (m/s)'); axis tight;
xlabel('Minutes')
% print -dpng -r0 velocity

figure
subplot(211)
plot(dt,a(:,10))
grid on; ylabel('p_{c} (s)')
title('Clock Bias')
subplot(212)
plot(dt(2:end),c*diff(a(:,10)),'r',dt,a(:,11),'b')
title('Clock Rate')
grid on; ylabel('v_{c} (m/s)')
xlabel('Minutes')
% print -dpng -r0 clock

% figure
% plot(dt,a(:,12:end))
% grid on;
% axis([0 max(dt) 0 20])
% xlabel('Minutes')
% legend('gdop','hdop','tdop','vdop','pdop')
% title('DOPS')
% print -dpng -r0 dops

