
clear all; close all;


fp = fopen('/tmp/GPSPIPE','r')
pts = 4096;
paxis = 1:pts/2;
edges = -2048:128:2048;

B = zeros(pts,1);

figure(1)
a = plot(paxis,B(1:2:end),'b',paxis,B(2:2:end),'r');    
axis([1 pts/2 -2^11 2^11])
grid on;

if(fp > 0)

while(1)

    %flush the pipe
    B = fread(fp,pts,'int16');
    
    figure(1)
    set(a(1),'Ydata',B(1:2:end));
    set(a(2),'Ydata',B(2:2:end));    
    drawnow;
    pause(0.01);
    
end

end