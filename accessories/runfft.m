function [output] = runfft(input, pts, repeats)

pts = length(input);

%remove scaling?
X = input*32767/max(max([abs(real(input)) abs(imag(input))]));
Y(1:2:pts*2) = real(X);
Y(2:2:pts*2) = imag(X);

fp = fopen('input.dat','wb');
fwrite(fp,Y,'int16');
fclose(fp);

str = sprintf('./FFT_Test.exe %d %d\n', pts, repeats);
system(str);

fp = fopen('output.dat','rb');
T = fread(fp,inf,'int16');
fclose(fp);

T = double(T);
T = T(1:2:end) + i*T(2:2:end);
output = T;
