%function [] = gen_fft_codes(bits)
bits = 9;

% Generate the raw codes (+-1)
prn = prn_gen();

% Resample the codes to 2.048e6 Msps
dt = round(linspace(1,1023,2048));
prn = prn(dt,:);

% Compute FFT of codes
prn = fft(prn,[],1);

% ind = bitrevorder(0:2047)+1;
% prn = prn(ind,:);

% Conjugate codes
prn = conj(prn);

% Scale codes to 9 signed bits
scale = (2^bits)/max(abs(prn(:)));
prn = prn*scale;

% Seperate real and imaginary parts, round towards zero for each
prn_r = round(real(prn));    prn_i = round(imag(prn));
prn_r = int16(prn_r);        prn_i = int16(prn_i);

% First unwrap the vectors
prn_r = reshape(prn_r,[1 2048*51]);
prn_i = reshape(prn_i,[1 2048*51]);

% Open the .h file
fp = fopen('prn_codes.h','wt');

% Write out the values
fprintf(fp,'#ifndef PRN_CODES_H_\n');
fprintf(fp,'int16 PRN_Codes[208896] = {');
for(lcv = 1:length(prn_i)-1)
    fprintf(fp,'%0d,%0d,\n',prn_r(lcv),prn_i(lcv));
end
lcv = lcv+1;
fprintf(fp,'%0d,%0d};',prn_r(lcv),prn_i(lcv));
fprintf(fp,'#endif\n');
fprintf(fp,'\n\n\n');
fclose(fp);
