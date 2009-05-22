clear; clc; close all;

[data] = get_channel('channel.log');

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).cn0,'.','MarkerSize',1);
end

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).sv,'o');
end

figure
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).code_nco-1.023e6,'.','MarkerSize',1);
end

for(lcv = 1:32)
    plot((data(lcv).carrier_nco-160000)*(1.023e6/1.57542e9),'k');
end