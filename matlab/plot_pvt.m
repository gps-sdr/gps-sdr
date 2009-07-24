clear; clc; close all;
pause(1)

% Grab the pvt only
[data] = get_pvt('pvt.log');

% p(p == 0) = NaN;
% v(v == 0) = NaN;

% Plot nsv, position, velocity
figure
subplot(311)
plot(data.nsv)
hold on; grid on;
subplot(312)
plot(data.p)
hold on; grid on;
axis([1 length(data.p) -1e8 1e8])
subplot(313)
plot(data.v)
hold on; grid on;
axis([1 length(data.p) -1e4 1e4])
