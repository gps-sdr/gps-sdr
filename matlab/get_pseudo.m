function [data] = get_pseudo(filename)

% fprintf(lfile,"%d,%d,%d,%d,%.16e,%.16e,%.16e,%.16e,%.16e\n",
% PSEUDORANGE_M_ID,
% pPseudo->tic,
% _chan,
% pPseudo->sv,
% pPseudo->meters,
% pPseudo->meters_rate,
% pPseudo->residual,
% pPseudo->residual_rate,
% pPseudo->uncorrected);

a = csvread(filename);

for(sv = 0:31)
    ind = find(a(:,4) == sv);
    b = a(ind,:);
    data(sv+1).tic = b(:,2);
    data(sv+1).meters = b(:,5);
    data(sv+1).meters_rate = b(:,6);
    data(sv+1).residual = b(:,7);
    data(sv+1).residual_rate = b(:,8);
    data(sv+1).uncorrected = b(:,9);
end