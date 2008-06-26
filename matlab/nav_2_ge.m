clear; 
clc;
close all;

path = '/home/gwheckler/workspace/gps-sdr/'
fname = [path,'navigation.tlm'];

a = dlmread(fname);

x = a(:,4); y = a(:,5); z = a(:,6);

%Remove bad pvts
ind = find(x ~= 0);
x = x(ind);
y = y(ind);
z = z(ind);

%Convert to lat, long, alt
[phi, lambda, h] = ecef2geodetic (x,y,z,[6378137.0 0.081819190842622]);

%Make degrees
phi = 180*phi/pi;
lambda = 180*lambda/pi;

ge_output('nav.kml',ge_plot3(lambda,phi,h,'lineColor','#FF0000FF'));