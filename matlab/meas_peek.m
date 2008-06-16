close('all','force'); clear; clc;
pause(1.0);

% pMeas->chan,
% pMeas->code_phase_mod,
% pMeas->_1ms_epoch,
% pMeas->_20ms_epoch,
% pMeas->_z_count,
% pMeas->count,
% pMeas->navigate;

c = 2.99792458e8;

fname = sprintf('../measurement.tlm');
a = dlmread(fname);
len = floor(length(a)/12);
a = a(1:12*len,:);

code_phase = reshape(a(:,2),[12 len]).';
ms1 = reshape(a(:,3),[12 len]).';
ms20 = reshape(a(:,4),[12 len]).';
zc = reshape(a(:,5),[12 len]).';
count = reshape(a(:,6),[12 len]).';

code_time = code_phase / 1.023e6 + .001 * ms1 + .02 * ms20;
code_time = code_time*c;
%code_time = .001 * ms1 + .02 * ms20;
code_time = mod(code_time, 0.1);

plot(code_time)
