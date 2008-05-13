function PRN_Source = PRNGen();

% file = fopen('PRN_Codes.dat','wb');

%Setup
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
G1 = zeros(1023,1);
G2 = zeros(1023,10);
G1_register = ones(10,1);
G2_register = ones(10,1);
registertemp = zeros(9);
taps1 = [1, 2, 3, 4, 0, 1, 0, 1, 2, 1, 2, 4, 5, 6, 7, 8, 0, 1, 2, 3, 4, 5, 0, 3, 4, 5, 6, 7, 0, 1, 2, 3] + 1;
taps2 = [5, 6, 7, 8, 8, 9, 7, 8, 9, 2, 3, 5, 6, 7, 8, 9, 3, 4, 5, 6, 7, 8, 2, 5, 6, 7, 8, 9, 5, 6, 7, 8] + 1;
feedback1 = 0;
feedback2 = 0;
A = zeros(1023,32);
C = zeros(1023,32);
B = zeros(1023,32);
D = zeros(1023,32);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Being RPN Generation
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
for(lcv = 1:1023)
    G1(lcv) = G1_register(10);
    feedback1 = xor(G1_register(3),G1_register(10));
    G1_register = [feedback1; G1_register(1:9,1)];
    
    G2(lcv,:) = G2_register';
    feedback2 = mod(G2_register(2)+G2_register(3)+G2_register(6)+G2_register(8)+G2_register(9)+G2_register(10),2);
    G2_register = [feedback2; G2_register(1:9,1)];
    
end


for(lcv2 = 1:32)
        A(:,lcv2) = G1(:);
        B(:,lcv2) = G2(:,taps1(lcv2));
        C(:,lcv2) = G2(:,taps2(lcv2));
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%Put into -1's and 1's
D = (2 * mod(A+B+C,2)) - 1; 

%write out the PRNs
% for(lcv = 1:32)
%     fwrite(file,D(:,lcv),'int16');
% end

%Return the PRNs
PRN_Source = D;

% fclose(file);

return;

    