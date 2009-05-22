function [data] = get_spirent_pseudo(fname, t0)

a = csvread(fname,2,0);

b = a.';
b = b(:);

for(sv = 1:32)    
    [ind] = find(b == sv);
    [i, j] = ind2sub(size(a.'),ind);
    data(sv).time = a(j,1) + t0;
    data(sv).elev = b(ind+1);
    data(sv).azim = b(ind+2);
    data(sv).dbw =  b(ind+3);
    data(sv).range = b(ind+4);
    data(sv).pseudo = b(ind+5);
    data(sv).pseudo_rate = b(ind+6);
end
