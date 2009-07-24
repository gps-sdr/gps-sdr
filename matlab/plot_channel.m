clear; clc; close all; pause(1)

[data] = get_channel('channel.log');

figure
pause(1)
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).cn0,'.','MarkerSize',1);
end

figure
pause(1)
hold all; grid on;
for(lcv = 1:32)
    plot(data(lcv).tic,data(lcv).sv,'o');
end
