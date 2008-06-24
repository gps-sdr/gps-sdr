% Compare PVT output of receiver to Spirent truth data

close all;
clear all;

%THESE PARAMETERS ARE SIMULATION DEPENDENT!!
gps_utc_offset = 12;

t0 = 12;    %for leo_motion.csv
truthFile = 'leo_motion.csv';

%t0 = 432012;    %for static_motion.csv
%truthFile = 'static_motion.csv';

% Default navigation file
path = '/home/gheckler/GPS_Data/Car_Test3/'
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
navDataOld = csvread(navFile);

%Create a vector of times that need to be interpolated.  Make sure they are
%the times with a valid GPS measurement
goodSolution    = (find(navDataOld(:,2) >= 4));
oldClock        = navDataOld(goodSolution,12);
oldNonIntNav    = navDataOld(goodSolution,4:9);      
svs             = navDataOld(goodSolution,2);
 
%Find coverage gaps
gaps = find(diff(oldClock) > 1.1);
numGaps = length(gaps-1);  %This should remove data at the end of the file

%if there are coverage gaps, then determine the areas of good coverage.
currentClock = floor(oldClock(1));
newTime = [];
oldTime = [];

if(numGaps > 0)
   
    for i = 1:numGaps  
        
        if(i > 1) 
            
            tmp = currentClock:1:currentClock + (gaps(i)-gaps(i-1)) - 1;
            tmp2 = oldClock(gaps(i-1)+1):oldClock(gaps(i))+1;
            oldNav = cat(1,oldNav,oldNonIntNav ( (gaps(i-1)+1) :gaps(i) ,1:6));
            satsTracked = cat(1,satsTracked,svs( (gaps(i-1)+1) :gaps(i) ,1));
            
        else  %Before the first gap
            
            tmp  = currentClock:1:currentClock + gaps(i) -1 ;
            tmp2 = oldClock(1):oldClock(gaps(i))+1;
            oldNav = oldNonIntNav (1:(gaps(i)),1:6);
            satsTracked = svs(1:(gaps(i)),1);
            
        end
        
        oldTime = cat(2,oldTime,tmp2);
        newTime = cat(2,newTime,tmp);
        currentClock = floor(oldClock(gaps(i)+1));       
    
    end
        
    %Handle the case where there are good measurements after the last gap
    tmp    = currentClock:1:currentClock + length(oldClock(gaps(i)+1):oldClock(end));
    tmp2   = oldClock(gaps(i)+1):oldClock(end)+1;
    oldNav = cat(1,oldNav,oldNonIntNav ( (gaps(i)+1) :end ,1:6));
    satsTracked = cat(1, satsTracked,svs ( (gaps(i)+1) :end ,1));

    oldTime = cat(2,oldTime,tmp2);
    newTime = cat(2,newTime,tmp);
    
else
    
    oldTime = oldClock;
    newTime = round(oldClock(1)):1:oldClock(1)+length(oldClock)-1;
    oldNav  = oldNonIntNav;
    
end 


%% Perform the iteration to align with GPS time
fprintf(1,'Interpolating the results to GPS time\n');
newNav = interp1(oldTime,oldNav,newTime,'spline');
 

%% Create a output matrix that would match the input matrix from Spirent
fprintf(1,'Generating Spirent compatible Data Format\n');
outputMat = zeros(length(truth),7);
for i = 1:1:length(truth)
   
    outputMat(i,1) = t0 + i - 1 + gps_utc_offset; %one second data assumption
    
    %Determine if we have a good measurement for this time period
    tmp = find(newTime(:) == (t0+i-1));
   
    if(isempty(tmp))
        %There is no data, so put in zeros
        outputMat(i,2:8) = zeros(1,7);
    else
        outputMat(i,2:8) = [newNav(tmp,1:6), satsTracked(tmp)];   
    end
    
end


% Ugly Patch
offset = gps_utc_offset + (length(outputMat)-length(navDataOld)) + 1;
outputMat(1:length((navDataOld(offset:end,3))),8) = (navDataOld(offset:end,3));

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
plot(errorMat(:,2),'b.','MarkerSize',3)
hold on; grid on;
plot(errorMat(:,3),'r.','MarkerSize',3)
plot(errorMat(:,4),'k.','MarkerSize',3)
plot(perr,'g.','MarkerSize',3)
ylabel('Position Error (m)')
% legend('p_{x}','p_{y}','p_{z}','p_{rms}')
    
subplot(312)
plot(100*errorMat(:,5),'b','MarkerSize',3)
hold on; grid on;
plot(100*errorMat(:,6),'r','MarkerSize',3)
plot(100*errorMat(:,7),'k','MarkerSize',3)
plot(100*verr,'g.','MarkerSize',3)
ylabel('Velocity Error (cm/s)')
% legend('v_{x}','v_{y}','v_{z}','v_{rms}')
    
subplot(313)
plot(outputMat(:,8))
grid on
