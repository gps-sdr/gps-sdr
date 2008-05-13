%FFT.m
clear;
close all; pause(0.5)

pts = 64;
repeats = 1000000;
xaxis = 0:pts-1;

%Generate vector
%-----------------------------------
x = (rand(1,pts)-0.5) + i*(rand(1,pts)-0.5);
%x = [1 2 3 4];

%x = exp(-2*pi*j*.235*[0:pts-1]);
%-----------------------------------

%Truth
%-----------------------------------
A = fft(x);
%A = x;
%-----------------------------------


%runfft(input, pts, start, stop, scale, stride, repeats);
B = runfft(x, pts, repeats);

A = A*(max(abs(B))/max(abs(A)));
A = round(A);
A = A.';

figure
subplot(211)
plot(real(A),'bs')
hold on; grid on;
plot(real(B),'r')
axis tight;
subplot(212)
plot(imag(A),'bs')
hold on; grid on;
plot(imag(B),'r')
axis tight;


