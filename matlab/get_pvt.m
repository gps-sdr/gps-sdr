function [data] = get_pvt(filename)

a = csvread(filename);

m1 = a(:,1) == 4;
ind = find(m1);
b = a(ind,:);

data.tic = b(:,2);
data.nsv = b(:,3);
data.conv = b(:,4);
data.t = b(:,5);
data.p = b(:,6:8);
data.v = b(:,9:11);
data.g = b(:,12);


