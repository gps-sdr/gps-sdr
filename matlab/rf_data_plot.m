clear; clc; close all;

fp = fopen('../crap.dat','r');
fseek(fp, -2.048e6*4, 'eof');
a = fread(fp, inf, 'int16');
fclose(fp);

taps = [4 5 6 7 6 5 4];
taps2 = [0.014784326154395  -0.040318556476920  -0.058368784627852   0.003050930049134   0.134750000850273   0.270021541864534   0.327275642655652   0.270021541864534   0.134750000850273   0.003050930049134  -0.058368784627852  -0.040318556476920 0.014784326154395];

a =  a(1:2:end) + j*a(2:2:end);

ind = [0:length(a)-1]*(2.048e6/8.1838e6);
ind = floor(ind);
ind = find(diff(ind) ~= 0);

b = filter(floor(taps2*24), 1, a);
b = b(ind);

bins = [-2^8:2^8];

hra = histc(real(a), bins);
hrb = histc(real(b), bins);

figure
hold all; grid on;
subplot(211)
bar(bins,hra./length(a))
subplot(212)
bar(bins,hrb./length(b))

figure
psd(a,1024)

figure
psd(b,1024)