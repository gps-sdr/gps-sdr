function [] = parse_log(filename, path)

if(path == [])
    path = '.';
end

% enum CCSDS_MESSAGES_IDS
% {
% 	FIRST_M_ID,
% 	FIRST_PERIODIC_M_ID,
% 	BOARD_HEALTH_M_ID,
% 	TASK_HEALTH_M_ID,
% 	SPS_M_ID,
% 	TOT_M_ID,
% 	PPS_M_ID,
% 	CLOCK_M_ID,
% 	CHANNEL_M_ID,
% 	SV_POSITION_M_ID,
% 	MEASUREMENT_M_ID,
% 	PSEUDORANGE_M_ID,
% 	SV_PREDICTION_M_ID,
% 	LAST_PERIODIC_M_ID,
% 	EKF_STATE_M_ID,
% 	EKF_COVARIANCE_M_ID,
% 	EKF_RESIDUAL_M_ID,
% 	COMMAND_M_ID,
% 	COMMAND_ACK_M_ID,
% 	EPHEMERIS_M_ID,
% 	ALMANAC_M_ID,
% 	EPHEMERIS_STATUS_M_ID,
% 	SV_SELECT_STATUS_M_ID,
% 	EEPROM_M_ID,
% 	EEPROM_CHKSUM_M_ID,
% 	MEMORY_M_ID,
% 	MEMORY_CHKSUM_M_ID,
% 	LAST_M_ID
% };


% Message codes
pvt_code = 4;
clock_code = 7;
ekf_state_code = 14;
ekf_cov_code = 15;
ekf_resid_code = 16;
channel_code = 8;
pseudo_code = 11;
prediction_code = 12;

% Run the parse with grep
fprintf('Parsed for PVT\n');
str = sprintf('grep %s -e "^%d" > %s/pvt.log\n',filename,pvt_code,path);
system(str);

fprintf('Parsed for Clock\n');
str = sprintf('grep %s -e "^%d" > %s/clock.log\n',filename,clock_code,path);
system(str);

fprintf('Parsed for Pseudoranges\n');
str = sprintf('grep %s -e "^%d" > %s/pseudo.log\n',filename,pseudo_code,path);
system(str);

fprintf('Parsed for Channel\n');
str = sprintf('grep %s -e "^%d" > %s/channel.log\n',filename,channel_code,path);
system(str);

fprintf('Parsed for Prediction\n');
str = sprintf('grep %s -e "^%d" > %s/prediction.log\n',filename,prediction_code,path);
system(str);

fprintf('Parsed for EKF State\n');
str = sprintf('grep %s -e "^%d" > %s/ekf_state.log\n',filename,ekf_state_code,path);
system(str);

fprintf('Parsed for EKF Covariance\n');
str = sprintf('grep %s -e "^%d" > %s/ekf_cov.log\n',filename,ekf_cov_code,path);
system(str);

fprintf('Parsed for EKF Residual\n');
str = sprintf('grep %s -e "^%d" > %s/ekf_res.log\n',filename,ekf_resid_code,path);
system(str);

