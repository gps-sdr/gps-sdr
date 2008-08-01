% Compare PVT output of receiver to Spirent truth data

close all;
clear all;

%THESE PARAMETERS ARE SIMULATION DEPENDENT!!
gps_utc_offset = 12;

t0 = 12;    %for leo_motion.csv
truthFile = '../leo_motion.csv';

%t0 = 432012;    %for static_motion.csv
%truthFile = 'static_motion.csv';

% Default navigation file
path = '../'
fname = [path,'navigation.tlm'];

%% Prepare the Spirent Data

% Open the truth file, skipping the first two rows because they are text
% headers
oldTruth = csvread(truthFile,2,0);

% The truth data is assumed to occur at 1 Hz, offset from t0, the initial
% GPS time.  Create a new matrix with time based upon the GPS measurement
truth = zeros(length(oldTruth),7);
truth(:,1) = t0 + oldTruth(:,1)/1000 + gps_utc_offset;

%Currently we are only worried about position, so extract the position
%states
truth(:,2:7) = oldTruth(:,2:7);


%% Prepare the Receiver Data

%Open up the navigation file
pvt = csvread(fname);
% navDataOld = navDataOld(1:10:end,:);

%Create a vector of times that need to be interpolated.  Make sure they are
%the times with a valid GPS measurement
good_pvt    = find(pvt(:,2) >= 4);
clock       = pvt(good_pvt,12);
pvt_xyz     = pvt(good_pvt,4:9);      
svs         = pvt(good_pvt,2);
 
new_clock = ceil(clock);


%% Perform the iteration to align with GPS time
fprintf(1,'Interpolating the results to GPS time\n');
new_xyz = interp1(clock,pvt_xyz,new_clock,'spline');
 
satsTracked = svs;

%% Create a output matrix that would match the input matrix from Spirent
fprintf(1,'Generating Spirent compatible Data Format\n');
outputMat = zeros(length(truth),7);
for i = 1:1:length(truth)
   
    outputMat(i,1) = t0 + i - 1 + gps_utc_offset; %one second data assumption
    
    %Determine if we have a good measurement for this time period
    tmp = find(new_clock(:) == (t0+i-1));
   
    if(isempty(tmp))
        %There is no data, so put in zeros
        outputMat(i,2:8) = zeros(1,7);
    else
        outputMat(i,2:8) = [new_xyz(tmp,1:6), svs(tmp)];   
    end
    
end


% Ugly Patch
offset = gps_utc_offset + (length(outputMat)-length(pvt_xyz)) + 1;
outputMat(1:length((pvt_xyz(offset:end,3))),8) = (pvt_xyz(offset:end,3));

%% Perform the error analysis
fprintf(1,'Performing the error analysis\n');

% Diff truth from sltn
errorMat(:,2:7) = truth(:,2:7) - outputMat(:,2:7);

% Mask out times receiver was not outputting a sltn
mask = outputMat(:,2) == 0;
errorMat(mask,2:end) = NaN;

%% Calculate and display errors
perr = sqrt(sum(errorMat(:,2:4).^2,2));
verr = sqrt(sum(errorMat(:,5:7).^2,2));

figure(1)
subplot(311)
plot(errorMat(:,2),'b.','MarkerSize',5)
hold on; grid on;
plot(errorMat(:,3),'r.','MarkerSize',5)
plot(errorMat(:,4),'k.','MarkerSize',5)
plot(perr,'g.','MarkerSize',5)
ylabel('Position Error (m)')
% legend('p_{x}','p_{y}','p_{z}','p_{rms}')
    
subplot(312)
plot(100*errorMat(:,5),'b','MarkerSize',5)
hold on; grid on;
plot(100*errorMat(:,6),'r','MarkerSize',5)
plot(100*errorMat(:,7),'k','MarkerSize',5)
plot(100*verr,'g.','MarkerSize',5)
ylabel('Velocity Error (cm/s)')
% legend('v_{x}','v_{y}','v_{z}','v_{rms}')
    
subplot(313)
plot(outputMat(:,8))
grid on
