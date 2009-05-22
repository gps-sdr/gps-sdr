function [data] = get_prediction(filename)

% /* Pseudorange */
% fprintf(lfile,"%d,%d,%d,%d,%.16e,%.16e,%.16e,%.16e,%.16e,%.16e\n",
% SV_PREDICTION_M_ID,
% pNav->tic,
% _sv,
% pSV->visible,
% pSV->time,
% pSV->delay,
% pSV->doppler,
% pSV->doppler_rate,
% pSV->elev,
% pSV->azim);

a = csvread(filename);

for(sv = 0:31)
    ind = find(a(:,3) == sv);
    b = a(ind,:);
    data(sv+1).tic = b(:,2);
    data(sv+1).vis = b(:,4);
    data(sv+1).time = b(:,5);
    data(sv+1).delay = b(:,6);
    data(sv+1).doppler = b(:,7);
    data(sv+1).doppler_rate = b(:,8);
    data(sv+1).elev = b(:,9);
    data(sv+1).azim = b(:,10);
end
