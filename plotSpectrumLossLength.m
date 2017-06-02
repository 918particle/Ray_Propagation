pkg load signal

lower_band = 0.02;
upper_band = 1.0;
delta_f = 0.02; %GHz
frequency_bins = linspace(lower_band,upper_band,floor((upper_band-lower_band)/delta_f));

distance_long = 543.0;
distance_short = 23.0;
sigma_distance_short = 0.005;
distance_factor = 10*log10(distance_long/distance_short);

calibration_start_sample_air = 12000;
calibration_length_air = 3000;
calibration_start_sample_snow = 12000;
calibration_length_snow = 3000;
calibration_downsample = 16;

prompt_component_start_sample = 9650;
prompt_component_length = 3000;
prompt_downsample = 8;

atten_20dB = 20.0;
atten_3dB = 3.0;

linew = 3.0;
fon = "Courier";
fonts = 20.0;
linewc = 0.0;
linewc2 = 4.0;

%Surface propagation data
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
s_prompt = s(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
t_prompt = t(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
s_prompt = downsample(s_prompt,prompt_downsample);
t_prompt = downsample(t_prompt,prompt_downsample);
dt_prompt = t_prompt(2)-t_prompt(1); %Units: nanoseconds
fs_prompt = 1.0/dt_prompt; %Units: GHz
fc_prompt = fs_prompt/2.0; %Units: GHz
f_prompt = linspace(0,fc_prompt,length(t_prompt)/2); %Units: GHz

%Calibration data
inAirCalibration = load('surface_calibration_air.dat');
inSnowCalibration = load('surface_calibration_snow.dat');
t_cal_air = inAirCalibration(calibration_start_sample_air:calibration_start_sample_air+calibration_length_air,1)*1e9; %Units: nanoseconds
t_cal_air_down = downsample(t_cal_air,calibration_downsample); %Units: nanoseconds
t_cal_snow = inSnowCalibration(calibration_start_sample_snow:calibration_start_sample_snow+calibration_length_snow,1)*1e9; %Units: nanoseconds
t_cal_snow_down = downsample(t_cal_snow,calibration_downsample); %Units: nanoseconds
s_cal_air = inAirCalibration(calibration_start_sample_air:calibration_start_sample_air+calibration_length_air,2);
s_cal_air_down = downsample(s_cal_air,calibration_downsample);
s_cal_snow = inSnowCalibration(calibration_start_sample_snow:calibration_start_sample_snow+calibration_length_snow,2);
s_cal_snow_down = downsample(s_cal_snow,calibration_downsample);
dt_cal_air = t_cal_air(2)-t_cal_air(1); %Units: nanoseconds
dt_cal_air_down = t_cal_air_down(2)-t_cal_air_down(1); %Units: nanoseconds
dt_cal_snow = t_cal_snow(2)-t_cal_snow(1); %Units: nanoseconds
dt_cal_snow_down = t_cal_snow_down(2)-t_cal_snow_down(1); %Units: nanoseconds
fs_cal_air = 1.0/dt_cal_air; %Units: GHz
fs_cal_air_down = 1.0/dt_cal_air_down; %Units: GHz
fs_cal_snow = 1.0/dt_cal_snow; %Units: GHz
fs_cal_snow_down = 1.0/dt_cal_snow_down; %Units: GHz
fc_cal_air = fs_cal_air/2.0; %Units: GHz
fc_cal_air_down = fs_cal_air_down/2.0; %Units: GHz
fc_cal_snow = fs_cal_snow/2.0; %Units: GHz
fc_cal_snow_down = fs_cal_snow_down/2.0; %Units: GHz
f_cal_air = linspace(0,fc_cal_air,length(t_cal_air)/2); %Units: GHz
f_cal_air_down = linspace(0,fc_cal_air_down,length(t_cal_air_down)/2.0); %Units: GHz
f_cal_snow = linspace(0,fc_cal_snow,length(t_cal_snow)/2); %Units: GHz
f_cal_snow_down = linspace(0,fc_cal_snow_down,length(t_cal_snow_down)/2.0); %Units: GHz

%The power spectrum of the prompt component
figure(1);
hold on;
p1 = conj(fft(center(s_prompt))).*fft(center(s_prompt));
p1 = p1(1:length(f_prompt));
p2 = conj(fft(center(s_cal_air_down))).*fft(center(s_cal_air_down));
p2 = p2(1:length(f_cal_air_down));
p3 = conj(fft(center(s_cal_snow_down))).*fft(center(s_cal_snow_down));
p3 = p3(1:length(f_cal_snow_down));
%The final 20 dB just means DC noise is at 0 dB
p1 = 10*log10(p1)+distance_factor+20;
p2 = 10*log10(p2)+atten_3dB+20.0;
p3 = 10*log10(p3)+atten_20dB+atten_3dB+20.0;
p1_ave = zeros(size(frequency_bins));
p2_ave = zeros(size(frequency_bins));
p3_ave = zeros(size(frequency_bins));
p1_err = zeros(size(frequency_bins));
p2_err = zeros(size(frequency_bins));
p3_err = zeros(size(frequency_bins));
for i=1:(length(frequency_bins)-1)
	j = find(f_prompt>=frequency_bins(i),1,'first');
	k = find(f_prompt>=frequency_bins(i+1),1,'first');
	p1_ave(i) = mean(p1(j:k));
	p1_err(i) = std(p1(j:k))/sqrt(k-j);
endfor
for i=1:(length(frequency_bins)-1)
	j = find(f_cal_air_down>=frequency_bins(i),1,'first');
	k = find(f_cal_air_down>=frequency_bins(i+1),1,'first');
	p2_ave(i) = mean(p2(j:k));
	p2_err(i) = std(p2(j:k))/sqrt(k-j);
endfor
for i=1:(length(frequency_bins)-1)
	j = find(f_cal_snow_down>=frequency_bins(i),1,'first');
	k = find(f_cal_snow_down>=frequency_bins(i+1),1,'first');
	p3_ave(i) = mean(p3(j:k));
	p3_err(i) = std(p3(j:k))/sqrt(k-j);
endfor
frequency_bins+=delta_f/2.0;
p1_ave = 10.^(p1_ave/10.0); %prompt
p2_ave = 10.^(p2_ave/10.0); %air
p3_ave = 10.^(p3_ave/10.0); %snow
p1_err = 10.^(p1_err/10.0); %prompt
p2_err = 10.^(p2_err/10.0); %air
p3_err = 10.^(p3_err/10.0); %snow
q = log(p2_ave./p3_ave);
l = distance_short./q;
l_err = sqrt((sigma_distance_short/distance_short)^2+q.^2.*((p3_err./p3_ave).^2+(p2_err./p2_ave).^2));
errorbar(frequency_bins,l,l_err)
axis([0.1 0.3 -30.0 30.0]);

%set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
%xlabel("Frequency (GHz)");
%ylabel("Power (dB)");
%title("(0 dB = DC background)");
%herr1 = errorbar(frequency_bins,p1_ave,p1_err,'~-');
%set(herr1,'color','black');
%herr2= errorbar(frequency_bins,p2_ave,p2_err,'~-');
%set(herr2,'color','red');
%herr3 = errorbar(frequency_bins,p3_ave,p3_err,'~-');
%set(herr3,'color','blue');
%L = legend('Surface Prompt Component','Calibration (Air)','Calibration (Snow)','location','south');
%FL = findall(L,'-property','linewidth');
%set(FL,'linewidth',linew);
%set(gca,'tickLength',[0.025 0.025]);

%The saved graph.
print -dpdf 'March10_plot5.pdf'
