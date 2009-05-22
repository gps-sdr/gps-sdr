function [err, feedback, t, tic] = get_pps(filename)

% filename = '/home/gheckler/Desktop/overnight3.log';

a = csvread(filename);

m1 = a(:,1) == 25;
m2 = a(:,3) == 1;
mask = m1 & m2;
ind = find(mask);

b = a(ind,:);

err = b(:,4);
feedback = b(:,7);
t = b(:,2);
tic = b(:,2)
