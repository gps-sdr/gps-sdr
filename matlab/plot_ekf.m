clear; clc; close all;
pause(1)

if(0)
    filename = '/home/gheckler/Desktop/Logs/Parts/splitaa';
    path = '.';
    
    % Break the log into individual files
    parse_log(filename,path);
end

[tic, t, st, dt, p, v] = get_ekf('ekf_state.log');
[ticc, pc, vc] = get_covariance('ekf_cov.log');
[ticr, resid, status] = get_residual('ekf_res.log');

tic = 1:length(tic);

g = dec2bin(st);
c = int32(g(:,end)-48);

% Plot covariances
figure
subplot(211)
plot(ticc,pc)
hold on; grid on;
plot(ticc,-pc)
axis([min(ticc) max(ticc) -1000 1000])
legend('X','Y','Z','CB','Location','NorthWest')
subplot(212)
plot(ticc,vc)
hold on; grid on;
plot(ticc,-vc)
axis([min(ticc) max(ticc) -10 10])
legend('X','Y','Z','CR','Location','NorthWest')
pause(1)

%Plot residuals
figure
plot(ticr,resid)
axis([min(ticc) max(ticc) -1000 1000])
hold on; grid on;

figure
subplot(211)
plot(tic, dt)
axis([min(tic) max(tic) 0 15 ])
hold on; grid on;
ylabel('State Estimate Wall Time (sec)')
subplot(212)
plot(tic, floor(st*2^-16))
axis([min(tic) max(tic) 0 13])
hold on; grid on;
ylabel('SVs in Solution')
print -djpeg geonstime
