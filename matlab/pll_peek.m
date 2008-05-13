function [] = pll_peek(chan)
close all;

pts = 2000;
rds = 10;
chan = 0;

fp = fopen(sprintf('/tmp/CHAN%02dPIPE',chan),'r');
if(fp < 1)
    return;
end

% 	int32 header;
% 	int32 chan;
% 	int32 sv;
% 	int32 bit_lock;
% 	int32 frame_lock;
% 	int32 subframe;
%   int32 count;
% 	int32 len;	
% 	float I[3];
% 	float Q[3];
% 	float I_avg;
% 	float Q_var;
% 	float P_avg;
% 	float CN0;
% 	float code_nco;
% 	float carrier_nco;
% 	float w;
% 	float x;
% 	float z;	

w = zeros(pts,1);    
x = zeros(pts,1);    
z = zeros(pts,1);    
I = zeros(pts,1); 
Q = zeros(pts,1);

wi = 22;
xi = 23;
zi = 24;
Ii = 11;    
Qi = 14;    
    
while(1)
    

    A = fread(fp,24*rds,'float');

    if(size(A) ~= 0)
        
        
        len = floor(length(A)/24);
        A = A(1:len*24);
        A = reshape(A,[24 len]).';
        
        
        w(1:end-len) = w(len+1:end);
        x(1:end-len) = x(len+1:end);
        z(1:end-len) = z(len+1:end);
        I(1:end-len) = I(len+1:end);
        Q(1:end-len) = Q(len+1:end);        

        w(end-len+1:end) = A(:,wi);
        x(end-len+1:end) = A(:,xi);
        z(end-len+1:end) = A(:,zi);
        I(end-len+1:end) = A(:,Ii);
        Q(end-len+1:end) = A(:,Qi);
    
        figure(1)
%         subplot(411)
%         plot(w,'b.')
%         subplot(412)
%         plot(x,'b.')
        subplot(212)
        plot(z,'b')
        grid on;        
        subplot(211)
        plot(1:pts,Q,'r',1:pts,I,'b')
%         plot(1:pts,I,'b')        
        grid on;
%        drawnow
    end
    
    pause(0.01)
 
    
end  