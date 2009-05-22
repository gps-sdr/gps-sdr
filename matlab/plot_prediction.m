clear; clc; close all;
pause(1)

[data] = get_prediction('prediction.log');

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).time,data(lcv).doppler)
end

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).time,data(lcv).delay)
end

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).time,data(lcv).vis*lcv,'.')
end


