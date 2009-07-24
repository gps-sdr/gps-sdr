clear; clc; close all;
pause(1)

% Grab the pvt only
[data] = get_pvt('pvt.log');

ind = find(data.p(:,1) ~= 0);
data.p = data.p(ind,:);

%Convert to lat, long, alt
[phi, lambda, h] = ecef2geodetic (data.p(:,1),data.p(:,2),data.p(:,3),[6378137.0 0.081819190842622]);
 
%Make degrees
phi = 180*phi/pi;
lambda = 180*lambda/pi;

gfile = fopen('pvt.kml','wt');

fprintf(gfile,'<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n');
fprintf(gfile,'<kml xmlns=\"http://earth.google.com/kml/2.1\">\n');
fprintf(gfile,'<Document>\n');
fprintf(gfile,'<name>\n');
fprintf(gfile,'navigation.kml\n');
fprintf(gfile,'</name>\n');
fprintf(gfile,'<Placemark id=\"GPS SDR\">\n');
fprintf(gfile,'<name>\n');
fprintf(gfile,'GPS SDR\n');
fprintf(gfile,'</name>\n');
fprintf(gfile,'<visibility>\n');
fprintf(gfile,'1\n');
fprintf(gfile,'</visibility>\n');
fprintf(gfile,'<description>\n');
fprintf(gfile,'<![CDATA[]]>\n');
fprintf(gfile,'</description>\n');
fprintf(gfile,'<Style>\n');
fprintf(gfile,'<LineStyle>\n');
fprintf(gfile,'<color>\n');
fprintf(gfile,'#FF0000FF\n');
fprintf(gfile,'</color>\n');
fprintf(gfile,'<width>\n');
fprintf(gfile,'1.00\n');
fprintf(gfile,'</width>\n');
fprintf(gfile,'</LineStyle>\n');
fprintf(gfile,'<PolyStyle>\n');
fprintf(gfile,'<color>\n');
fprintf(gfile,'00ffffff\n');
fprintf(gfile,'</color>\n');
fprintf(gfile,'</PolyStyle>\n');
fprintf(gfile,'</Style>\n');
fprintf(gfile,'<Polygon id=\"poly_plot3\">\n');
fprintf(gfile,'<extrude>0</extrude>\n');
fprintf(gfile,'<altitudeMode>\n');
fprintf(gfile,'relativeToGround\n');
fprintf(gfile,'</altitudeMode>\n');
fprintf(gfile,'<outerBoundaryIs>\n');
fprintf(gfile,'<extrude>0</extrude>\n');
fprintf(gfile,'<LinearRing>\n');
fprintf(gfile,'<extrude>0</extrude>\n');
fprintf(gfile,'<tessellate>0</tessellate>\n');
fprintf(gfile,'<altitudeMode>\n');
fprintf(gfile,'absolute\n');
fprintf(gfile,'</altitudeMode>\n');
fprintf(gfile,'<coordinates>\n');
fprintf(gfile,'%15.9f,%15.9f,%15.9f\n',[lambda,phi,h].');
fprintf(gfile,'</coordinates>\n');
fprintf(gfile,'</LinearRing>\n');
fprintf(gfile,'</outerBoundaryIs>\n');
fprintf(gfile,'</Polygon>\n');
fprintf(gfile,'</Placemark>\n');
fprintf(gfile,'</Document>\n');
fprintf(gfile,'</kml>\n');

fclose(gfile);