% Compare PVT output of receiver to Spirent truth data

close all; clear all; clc;

t0 = 302400; 
truthFile = '/home/gheckler/workspace/GPM/Navigator_Core/matlab/motion_V1.csv';
testFile = 'ekf_state.log';

% EKF state file
[tice, t, st, dt, p, v] = get_ekf(testFile);

% Truth file
truth = csvread(truthFile,2,0);
truth = truth(:,1:7);

% Get times
truth_t = t0 + truth(:,1)/1000;
pvt_t = t;
pvt_r = floor(pvt_t);

% Interp the truth data to the pvt time tags
% pvt = interp1(pvt_t, pvt, pvt_r, 'spline');
truth = interp1(truth_t, truth, pvt_t, 'linear');

% Get the errors
pos_err = truth(:,2:4) - p(:,1:3);
vel_err = truth(:,5:7) - v(:,1:3);

% Calculate and display errors
perr = sqrt(sum(pos_err.^2,2));
verr = sqrt(sum(vel_err.^2,2));

pvt_t = 1:length(pvt_t);

% Plot position error
figure
subplot(211)
plot(pvt_t,pos_err(:,1),'b','MarkerSize',5)
hold on; grid on;
plot(pvt_t,pos_err(:,2),'r','MarkerSize',5)
plot(pvt_t,pos_err(:,3),'k','MarkerSize',5)
plot(pvt_t,perr,'g','MarkerSize',5)
axis([0 length(pvt_t) -1000 1000 ])
ylabel('Position Error (m)')
title(sprintf('%s',char(testFile)))
legend('p_{x}','p_{y}','p_{z}','p_{rms}')

% Plot velocity error
subplot(212)
plot(pvt_t,100*vel_err(:,1),'b','MarkerSize',5)
hold on; grid on;
plot(pvt_t,100*vel_err(:,2),'r','MarkerSize',5)
plot(pvt_t,100*vel_err(:,3),'k','MarkerSize',5)
plot(pvt_t,100*verr,'g','MarkerSize',5)
ylabel('Velocity Error (cm/s)')
axis([0 length(pvt_t) -100 100 ])
legend('v_{x}','v_{y}','v_{z}','v_{rms}')

