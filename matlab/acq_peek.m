% ACQ_PEEK Inspect the output from the acquisition process
%   ACQ_PEEK(sv, type) is used to visualize the instantaneous results from
%   the acquisition process. % If called with no arguments, defaults to a 32
%   PRN strong, medium, weak plot, update as the acquisition process goes 
%   through the constellation. If a sv number is given, the program will
%   either plot the 3-D delay doppler grid (type == 0), or a continuous plot
%   of the delay and Doppler estimate (type == 1).
function acq_look(varargin)

close all;

if(nargin == 0)
    ALLSV = 1;
elseif (nargin == 1)
    error('Requires 0 or 2 arguments!\n')
elseif (nargin == 2)
    ALLSV = 0;
    SV = varargin{1};
    TYPE = varargin{2};
else
    return;
end
    
if(ALLSV == 1)
    
    close all;

    %A = get_acq();
    power_s = zeros(32,1);
    power_m = zeros(32,1);
    power_w = zeros(32,1);
    
    A = get_acq();
    A(:,5) = A(:,5) / 1e7;

    mask_s = A(:,1) == 0;
    mask_m = A(:,1) == 1;
    mask_w = A(:,1) == 2;    
    power_s(mask_s) = A(mask_s,5);
    power_m(mask_m) = A(mask_m,5);    
    power_w(mask_w) = A(mask_w,5);        
    last_s = max(find(mask_s == 1));
    last_m = max(find(mask_m == 1));
    last_w = max(find(mask_m == 1));

    figure(1);

    subplot(311)
    a = stem(1:32,power_s,'b','FILLED');
    hold on;
    b = stem(last_s,power_s(last_s),'g','FILLED');
    grid on;      
    xlabel('Strong Acquisition')

    subplot(312)
    c = stem(1:32,power_m,'b','FILLED');
    hold on;
    d = stem(last_m,power_m(last_m),'g','FILLED');        
    grid on;
    xlabel('Medium Acquisition')

    subplot(313)
    e = stem(1:32,power_w,'b','FILLED');
    hold on;
    f = stem(last_w,power_m(last_w),'g','FILLED');        
    grid on;
    xlabel('Weak Acquisition')    
    
    while(1)

        A = get_acq();

        if(size(A) ~= 0)
            
            A(:,5) = A(:,5) / 1e7;
            
            mask_s = A(:,1) == 0;
            mask_m = A(:,1) == 1;
            mask_w = A(:,1) == 2;    
            power_s(mask_s) = A(mask_s,5);
            power_m(mask_m) = A(mask_m,5);    
            power_w(mask_w) = A(mask_w,5);        
            last_s = max(find(mask_s == 1));
            last_m = max(find(mask_m == 1));
            last_w = max(find(mask_m == 1));

            set(a,'Ydata',power_s);
            set(c,'Ydata',power_m);        
            set(e,'Ydata',power_w);                    

            set(b,'Xdata',last_s);
            set(d,'Xdata',last_m);
            set(f,'Xdata',last_w);            
            
            set(b,'Ydata',power_s(last_s));
            set(d,'Ydata',power_m(last_m));
            set(f,'Ydata',power_w(last_w));            

        end

        pause(0.1);

    end

else
    
    vsize = 200;
    
    if(TYPE == 1)

        B = get_acq();
        delay = zeros(vsize,1);
        doppler = zeros(vsize,1);
        power = zeros(vsize,1);
        pd = zeros(vsize,1);
        paxis = 1:vsize;
        k  = 0;
        
        while(1)
        
            A = get_acq();
            
            if(size(A) ~= 0)
                
                if(sum(A(:) ~= B(:)))

                B = A;
                
                delay(1:end-1) = delay(2:end);
                doppler(1:end-1) = doppler(2:end);
                power(1:end-1) = power(2:end);    
                pd(1:end-1) = pd(2:end);            

                delay(end) = A(SV,3);
                doppler(end) = A(SV,4);
                power(end) = A(SV,5);    

                tdelay = mode(delay.');
                ind1 = delay <= tdelay + 16;
                ind2 = delay >= tdelay - 16;        
                ind = ind1 & ind2;   
                
                pd(end) = sum(ind)/vsize;
                
                
                if(k == 0)
                    figure(1)
                    h = subplot(411)
                    a = plot(1:vsize,delay,'b.');
                    hold on;
                    b = plot(paxis(ind),delay(ind),'ro');
                    title(sprintf('Pd = %f\n, Td = %f\n',sum(ind)/vsize,tdelay),'FontSize',24);
                    grid on;
                    ylabel('Delay')            
                    axis([1 vsize 0 1023])

                    subplot(412)
                    c = plot(1:vsize,pd);
                    grid on;
                    ylabel('P_{D}')            

                    subplot(413)
                    d = plot(doppler);
                    hold on; grid on;
                    f = plot(doppler,'ro') ;                   
                    ylabel('Doppler')            

                    subplot(414)
                    e = plot(power);
                    grid on;
                    ylabel('Power')
                    k = k+1;
                else

                    set(a,'Ydata',delay);
                    set(b,'Ydata',delay(ind));        
                    set(c,'Ydata',pd);        
                    set(d,'Ydata',doppler);        
                    set(e,'Ydata',power);        
                    set(f,'Ydata',doppler(ind));                    

                    %set(a,'Xdata',last_s);
                    set(b,'Xdata',paxis(ind));
                    set(f,'Xdata',paxis(ind));                    
                    %set(c,'Xdata',last_s);
                    %set(d,'Xdata',last_m);
                    %set(e,'Xdata',last_s);
             
                    title(h,sprintf('Pd = %f\n, Td = %f\n',sum(ind)/vsize,tdelay),'FontSize',24);                    
                    
                    drawnow;
                
                end
                
                pause(0.01);
                
                end
                
                pause(0.01);

            end
            
            pause(0.01);
        
        end

    else
        
        
        while(1)
            
            A = get_acq_grid(SV, TYPE);

            if(size(A) ~= 0)

                surf(A)
                shading interp;
                drawnow;

            end
        
        end
       
    end
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        
        
            
            
      
    
    
end