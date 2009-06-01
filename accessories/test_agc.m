clear all; close all;

fp = fopen('/tmp/GPSPIPE','r');
pts = 4096;
paxis = 1:pts/2;
% edges = -2048:128:2048;

B = zeros(pts,1);
A = B;

low = 4;
high = 16;

bits = 5;
edges = [-2^(bits+1):1:2^(bits+1)];


scale = 1;

while(1)
    
    B = fread(fp,pts,'int16');

     mask = B < 0;
     A(mask)  = fix(2*B(mask)/scale-1.0);
     A(~mask) = fix(2*B(~mask)/scale+0.0);    
     
%    A = B;

    num = sum(abs(A) > 2^bits);
    
    
    if(num < low)
        scale = scale - 1;
    end
    
    
    if(num > high)
        scale = scale + 1;
    end
    
    if(scale == 0)
        scale = 1;
    end
    
    

    
    figure(1)
    subplot(211)
    plot(paxis,A(1:2:end),'b',paxis,A(2:2:end),'r');    
%    plot(10*log10(abs(fft(A))))
%     axis([0 pts/2 -2^(bits+1) 2^(bits+1)])
    title(sprintf('%f\t%f\t%f\n',num,scale,3*std(A)))
    grid on;
    subplot(212)
    plot(10*log10(abs(fft(B))))
    plot(edges,histc(A,edges));
    axis([min(edges) max(edges) 0 10*pts/length(edges)])
    grid on;
    drawnow;
    
end

