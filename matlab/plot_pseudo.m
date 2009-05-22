clear; clc; close all;

[data] = get_pseudo('pseudo.log');

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).residual,'.','MarkerSize',1)
end


figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).residual_rate,'.','MarkerSize',1)
end