function [A] = get_chan(chan)

% 	float header;
% 	float chan;
% 	float sv;
% 	float bit_lock;
% 	float frame_lock;
% 	float subframe;
% 	float best_epoch;
% 	float count;
% 	float len;	
% 	float I[3];
% 	float Q[3];
% 	float I_avg;
% 	float Q_var;
% 	float P_avg;
% 	float CN0;
% 	float code_nco;
% 	float carrier_nco;
% 	float fll_lock;
% 	float pll_lock;
% 	float fll_lock_ticks;
% 	float w;
% 	float x;
% 	float z;	

pts = 27;

fp = fopen(sprintf('../chan%02d.dat',chan),'rb');
A(:,1) = fread(fp,inf,'float'); 

len = floor(length(A)/pts);
A = A(1:len*pts);

A = reshape(A, [pts len]).';