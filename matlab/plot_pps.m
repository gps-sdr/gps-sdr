%newPPS routine
clear; clc;

%fp=fopen('outputPPS.txt','r');
fname = '/home/gheckler/Desktop/goes4pps.log';

[t scale] = textread(fname,'%f %s');

for(i = 1:1:length(scale))

    if(strcmp('us',scale(i)))
        t(i) = t(i)*1e-6;
    end

    if(strcmp('u',scale(i)))
        t(i) = t(i)*1e-6;
    end
    
end

t(t > .5) = t(t > .5) - 1.0;
t = t(1:end-1);
t = t*1e6;

xaxis = [1:length(t)]/(60);

figure(1)
plot(xaxis, t)
axis([min(xaxis) max(xaxis) -100 100])
grid on;