% PSEUDO_PEEK Inspect the pseudoranges
%   PSEUDO_PEEK() is used to inspect the pseudoranges, plots include:
%   1: Pseudorange residuals

close('all','force'); clear; clc

% pPseudo->chan,
% pPseudo->time,
% pPseudo->time_rate,
% pPseudo->meters,
% pPseudo->meters_rate,
% pPseudo->residual,
% pPseudo->rate_residual,
% pPseudo->time_uncorrected

c = 2.99792458e8;

fname = sprintf('../pseudorange.tlm');
a = dlmread(fname);
len = floor(length(a)/12);
a = a(1:12*len,:);
dt = [1:len]/60;

resid = reshape(a(:,6),[12 len]).';
rresid = reshape(a(:,7),[12 len]).';

figure
plot(dt, resid)
% axis([1 max(dt) -200 200])
grid on;
ylabel('Residual (m)')
xlabel('Minutes')
title('Pseudorange Residuals')
% print -dpng -r0 residuals.png

figure
plot(dt, rresid)
% axis([1 max(dt) -200 200])
grid on;
ylabel('Residual (m)')
xlabel('Minutes')
title('Pseudorange Residuals')
% print -dpng -r0 residuals.png

