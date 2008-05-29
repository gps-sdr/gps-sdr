function prn = prngen();

% G2 Delays as defined in GPS-ISD-200D
delays = 1023 - [5, 6, 7, 8, 17, 18, 139, 140, 141, 251, 252, 254 ,255, 256, 257, 258, 469, 470, 471, 472, ...
    473, 474, 509, 512, 513, 514, 515, 516, 859, 860, 861, 862, 145, 175, 52, 21, 237, 235, 886, 657, 634, 762, ...
    355, 1012, 176, 603, 130, 359, 595, 68, 386];

% Setup
G1 = zeros(1023,1);
G2 = zeros(1023,1);
G1_register = ones(1,10);
G2_register = ones(1,10);
feedback1 = 0;
feedback2 = 0;

% Being PRN Generation
for(lcv = 1:1023)
    
    G1(lcv) = G1_register(1);
    G2(lcv) = G2_register(1);
    
    feedback1 = xor(G1_register(8),G1_register(1));
    feedback2 = mod(G2_register(9)+G2_register(8)+G2_register(5)+G2_register(3)+G2_register(2)+G2_register(1),2);
    
    G1_register = [G1_register(2:10), feedback1];
    G2_register = [G2_register(2:10), feedback2];
end


for(lcv = 1:51)
    ind = mod([1:1023] + delays(lcv) - 1,1023)+1;
    prn(:,lcv) = xor(G1(:),G2(ind));
end

prn = 2*prn-1;
    