function [tic, t, st, dt, p, v] = get_ekf(filename)

% 1: EKF_STATE_M_ID,
% 2: pEKF->tic,
% 3: pEKF->status,
% 4: pEKF->period,
% 5: pEKF->week,
% 6: pEKF->time,
% 7: pEKF->x,
% 8: pEKF->y,
% 9: pEKF->z,
% 10: pEKF->vx,
% 11: pEKF->vy,
% 12: pEKF->vz,
% 13: pEKF->solar,
% 14: pEKF->drag,
% 15: pEKF->clock_bias,
% 16: pEKF->clock_rate);
 
a = csvread(filename);

t = a(:,6);
tic = a(:,2);
st = a(:,3);
dt = a(:,4)*500e-6;
p = [a(:,7:9), a(:,15)];
v = [a(:,10:12), a(:,16)];
