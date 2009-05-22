function [tic, p, v] = get_covariance(filename)

a = csvread(filename);
b = a;
tic = b(:,2);
p = [b(:,3:5), b(:,11)];
v = [b(:,6:8), b(:,12)];
