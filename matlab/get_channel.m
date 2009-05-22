function [data] = get_channel(filename)

% fprintf(lfile,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
% CHANNEL_M_ID,
% pChan->tic,
% _chan,
% pChan->sv,
% pChan->count,
% pChan->bit_lock,
% pChan->frame_lock,
% pChan->subframe,
% pChan->len,
% pChan->p_avg,
% pChan->cn0,
% pChan->code_nco,
% pChan->carrier_nco);

HZ_2_NCO_CARR_INCR = (2.097152000000000e+03);
NCO_CARR_INCR_2_HZ = (4.768371582031250e-04);
HZ_2_NCO_CODE_INCR = (1.048576000000000e+03);
NCO_CODE_INCR_2_HZ = (9.536743164062500e-04);

a = csvread(filename);

for(sv = 0:31)
    ind = find(a(:,4) == sv);
    b = a(ind,:);
    ind = find(b(:,6) == 1);
    b = b(ind,:);
    cn0temp = b(:,11)/128;
%     cn0temp(cn0temp < 4866) = NaN;
%     cn0temp = cn0temp / 4096.0;
%     cn0temp = 10*log10((cn0temp - 1.0)./(20.0 - cn0temp)) + 30.25;
    
    data(sv+1).tic = b(:,2);
    data(sv+1).sv = b(:,4);
    data(sv+1).count = b(:,5);
    data(sv+1).bit_lock = b(:,6);
    data(sv+1).frame_lock = b(:,7);
    data(sv+1).subframe = b(:,8);
    data(sv+1).len = b(:,9);
    data(sv+1).p_avg = b(:,10);
    data(sv+1).cn0 = cn0temp;
    data(sv+1).code_nco = b(:,12)*NCO_CODE_INCR_2_HZ;
    data(sv+1).carrier_nco = b(:,13)*NCO_CARR_INCR_2_HZ;
end

