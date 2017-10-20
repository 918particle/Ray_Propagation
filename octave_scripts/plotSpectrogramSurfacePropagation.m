pkg load signal

lower_band = 0.2/1.29;
upper_band = 1.0;

distance_long = 543.0;
distance_short = 23.0;
distance_factor = 10*log10(distance_long/distance_short);

calibration_start_sample_air = 12000;
calibration_length_air = 3000;
calibration_start_sample_snow = 12000;
calibration_length_snow = 3000;
calibration_downsample = 8;

prompt_component_start_sample = 9650;
prompt_component_length = 3000;
prompt_downsample = 4;
delayed_component_start_sample = 11500;
delayed_component_length = 9999;
delayed_downsample = 4;

atten_20dB = 20.0;
atten_3dB = 3.0;
ampl_62dB = -62.5;

n = 500; %Length of time-slices, was 500
m = 25; %Number of overlap samples, was 25
n2 = 100;
m2 = 10;
linew = 5.0;
fon = "Courier";
fonts = 20.0;
linewc = 0.0;
linewc2 = 4.0;
maxFC = 0.8;
minT = 250.0;
linec = "none";

%Surface propagation data
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
dt = t(2)-t(1);
fs = 1.0/dt; %Units: nanoseconds
s = s+randn(size(s))*max(s)/10.0;

%The plot, linear
figure(1,'Position',[0,0,1600,1600]);
colormap(summer(250));
[S,F,T] = specgram(s,n,fs,hanning(n),m);
T = T+t(1);
S = log10(abs(S));
contourf(T,F,S,[-3:0.5:1],'linewidth',linewc,"linecolor",linec);
colorbar;
axisVector = [minT max(T) 0.1 maxFC];
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts)
cbh = findobj( gcf(), 'tag', 'colorbar');
set(cbh,"linewidth",linew,"fontname",fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Frequency (GHz)");
title("Log_{10}(|V()|)");
caxis([-3 1]);

%The saved graph.
print -dpng 'March9_plot1.png'

%The plot, dB
%colormap(summer(250));
%[S,F,T] = specgram(s,n,fs_prompt,hanning(n),m);
%T = T+t(1);
%contourf(T,F,10*log10(conj(S).*S)+100.0,'linewidth',linewc,"linecolor",linec);
%colorbar;
%axisVector = [minT max(T) 0.2 maxFC];
%axis(axisVector);
%set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts)
%cbh = findobj( gcf(), 'tag', 'colorbar');
%set(cbh,"linewidth",linew,"fontname",fon,"fontsize",fonts);
%xlabel("Time (ns)");
%ylabel("Frequency (GHz)");
%title("(dB Scale, 0 dB = average bkgd.)");
%caxis([0 200]);

%Calibration data
%inAirCalibration = load('surface_calibration_air.dat');
%inSnowCalibration = load('surface_calibration_snow.dat');
%t_cal_air = inAirCalibration(calibration_start_sample_air:calibration_start_sample_air+calibration_length_air,1)*1e9; %Units: nanoseconds
%t_cal_air_down = downsample(t_cal_air,calibration_downsample); %Units: nanoseconds
%t_cal_snow = inSnowCalibration(calibration_start_sample_snow:calibration_start_sample_snow+calibration_length_snow,1)*1e9; %Units: nanoseconds
%t_cal_snow_down = downsample(t_cal_snow,calibration_downsample); %Units: nanoseconds
%s_cal_air = inAirCalibration(calibration_start_sample_air:calibration_start_sample_air+calibration_length_air,2);
%s_cal_air_down = downsample(s_cal_air,calibration_downsample);
%s_cal_snow = inSnowCalibration(calibration_start_sample_snow:calibration_start_sample_snow+calibration_length_snow,2);
%s_cal_snow_down = downsample(s_cal_snow,calibration_downsample);
%dt_cal_air = t_cal_air(2)-t_cal_air(1); %Units: nanoseconds
%dt_cal_air_down = t_cal_air_down(2)-t_cal_air_down(1); %Units: nanoseconds
%dt_cal_snow = t_cal_snow(2)-t_cal_snow(1); %Units: nanoseconds
%dt_cal_snow_down = t_cal_snow_down(2)-t_cal_snow_down(1); %Units: nanoseconds
%fs_cal_air = 1.0/dt_cal_air; %Units: GHz
%fs_cal_air_down = 1.0/dt_cal_air_down; %Units: GHz
%fs_cal_snow = 1.0/dt_cal_snow; %Units: GHz
%fs_cal_snow_down = 1.0/dt_cal_snow_down; %Units: GHz
%fc_cal_air = fs_cal_air/2.0; %Units: GHz
%fc_cal_air_down = fs_cal_air_down/2.0; %Units: GHz
%fc_cal_snow = fs_cal_snow/2.0; %Units: GHz
%fc_cal_snow_down = fs_cal_snow_down/2.0; %Units: GHz
%f_cal_air = linspace(0,fc_cal_air,length(t_cal_air)/2); %Units: GHz
%f_cal_air_down = linspace(0,fc_cal_air_down,length(t_cal_air_down)/2.0); %Units: GHz
%f_cal_snow = linspace(0,fc_cal_snow,length(t_cal_snow)/2); %Units: GHz
%f_cal_snow_down = linspace(0,fc_cal_snow_down,length(t_cal_snow_down)/2.0); %Units: GHz

%s_prompt = s(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
%t_prompt = t(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
%s_delayed = s(delayed_component_start_sample:delayed_component_start_sample+delayed_component_length); %delayed component (11500:end)
%t_delayed = t(delayed_component_start_sample:delayed_component_start_sample+delayed_component_length); %delayed component (11500:end)
%s_prompt = downsample(s_prompt,prompt_downsample);
%t_prompt = downsample(t_prompt,prompt_downsample);
%s_delayed = downsample(s_delayed,delayed_downsample);
%t_delayed = downsample(t_delayed,delayed_downsample);
%dt_prompt = t_prompt(2)-t_prompt(1); %Units: nanoseconds
%fs_prompt = 1.0/dt_prompt; %Units: GHz
%fc_prompt = fs_prompt/2.0; %Units: GHz
%dt_delayed = t_delayed(2)-t_delayed(1); %Units: nanoseconds
%fs_delayed = 1.0/dt_delayed; %Units: GHz
%fc_delayed = fs_delayed/2.0; %Units: GHz
%f_prompt = linspace(0,fc_prompt,length(t_prompt)/2); %Units: GHz
%f_delayed = linspace(0,fc_delayed,length(t_delayed)/2); %Units: GHz
