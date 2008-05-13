function [] = meatball_peek(chan)
close all; 

pts = 500;
rds = 25;

fp = fopen(sprintf('/tmp/CHAN%02dPIPE',chan),'r');
if(fp < 1)
    return;
end

Ce = zeros(pts,1);    
Cp = zeros(pts,1);    
Cl = zeros(pts,1);    
    
    
    
while(1)
    

    A = fread(fp,16*rds,'float');

    if(size(A) ~= 0)
        
        
        len = length(A)/16;
        A = A(1:len*16);
        A = reshape(A,[16 len]).';
        
        
        Ce(1:end-len) = Ce(len+1:end);
        Cp(1:end-len) = Cp(len+1:end);
        Cl(1:end-len) = Cl(len+1:end);

        Ce(end-len+1:end) = A(:,2) + i.*A(:,5);
        Cp(end-len+1:end) = A(:,3) + i.*A(:,6);
        Cl(end-len+1:end) = A(:,4) + i.*A(:,7);
    
        plot(Cp,'b.')
        grid on;
        axis([-10000 10000 -10000 10000])
        
        drawnow
    end
    
    pause(0.04)
 
    
end  