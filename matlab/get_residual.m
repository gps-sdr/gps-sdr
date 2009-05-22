function [tic, resid, status] = get_residual(filename)

a = csvread(filename);
b = a;
tic = b(:,2);
resid = [b(:,3:14)];
status = [b(:,15:26)];