clc; close all;
pause(1)

sv = 7;
c = 2.99792458e8;
t0 = 172800; 

if(1)
    [data] = get_prediction('prediction.log');
    [spirent] = get_spirent_pseudo('/home/gheckler/Desktop/Logs/svdata.csv',t0);
end



% Interp the truth data to the pvt time tags
% pvt = interp1(pvt_t, pvt, pvt_r, 'spline');
tpr = interp1(spirent(sv).time, -spirent(sv).pseudo_rate*(1.57542e9/c), data(sv).time, 'spline');
tel = interp1(spirent(sv).time, spirent(sv).elev, data(sv).time, 'spline');

% Get the errors
dop_err = tpr - data(sv).doppler;
el_err = tel - data(sv).elev;

figure
subplot(211)
plot(data(sv).time,tpr,'b')
hold on; grid on;
plot(data(sv).time,data(sv).doppler,'r')
subplot(212)
hold on; grid on;
plot(data(sv).time, dop_err)


figure
subplot(211)
plot(data(sv).time,tel,'b')
hold on; grid on;
plot(data(sv).time,data(sv).elev,'r')
subplot(212)
hold on; grid on;
plot(data(sv).time, el_err)
