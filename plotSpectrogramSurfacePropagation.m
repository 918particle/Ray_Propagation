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
maxFC = 0.5;
minT = 320.0;
linec = "none";

%Surface propagation data
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
s_prompt = s(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
t_prompt = t(prompt_component_start_sample:prompt_component_start_sample+prompt_component_length); %prompt component (8500:11500)
s_delayed = s(delayed_component_start_sample:delayed_component_start_sample+delayed_component_length); %delayed component (11500:end)
t_delayed = t(delayed_component_start_sample:delayed_component_start_sample+delayed_component_length); %delayed component (11500:end)
s_prompt = downsample(s_prompt,prompt_downsample);
t_prompt = downsample(t_prompt,prompt_downsample);
s_delayed = downsample(s_delayed,delayed_downsample);
t_delayed = downsample(t_delayed,delayed_downsample);
dt_prompt = t_prompt(2)-t_prompt(1); %Units: nanoseconds
fs_prompt = 1.0/dt_prompt; %Units: GHz
fc_prompt = fs_prompt/2.0; %Units: GHz
dt_delayed = t_delayed(2)-t_delayed(1); %Units: nanoseconds
fs_delayed = 1.0/dt_delayed; %Units: GHz
fc_delayed = fs_delayed/2.0; %Units: GHz
f_prompt = linspace(0,fc_prompt,length(t_prompt)/2); %Units: GHz
f_delayed = linspace(0,fc_delayed,length(t_delayed)/2); %Units: GHz

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

%The plot, linear
%colormap(summer(250));
%[S,F,T] = specgram(s,n,fs,hanning(n),m);
%T = T+t(1);
%contourf(T,F,abs(S),'linewidth',linewc,"linecolor",linec);
%colorbar;
%axisVector = [minT max(T) 0 maxFC];
%axis(axisVector);
%set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts)
%cbh = findobj( gcf(), 'tag', 'colorbar');
%set(cbh,"linewidth",linew,"fontname",fon,"fontsize",fonts);
%xlabel("Time (ns)");
%ylabel("Frequency (GHz)");
%title("(Linear scale, relative voltage)");

%The plot, dB
%colormap(summer(250));
%[S,F,T] = specgram(s,n,fs,hanning(n),m);
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
%caxis([100 120]);

%The plot of prompt component, dB
%colormap(summer(250));
%[S,F,T] = specgram(s2,n2,fs,hanning(n2),m2);
%T = T+t2(1);
%contourf(T,F,10*log10(conj(S).*S)+100.0,'linewidth',linewc,"linecolor",linec);
%colorbar;
%axisVector = [minT max(T) 0.0 maxFC];
%axis(axisVector);
%set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts)
%cbh = findobj( gcf(), 'tag', 'colorbar');
%set(cbh,"linewidth",linew,"fontname",fon,"fontsize",fonts);
%xlabel("Time (ns)");
%ylabel("Frequency (GHz)");
%title("(dB Scale, 0 dB = average bkgd.)");
%caxis([40 140]);

%The power spectrum of the prompt component
figure(1);
hold on;
p1 = conj(fft(center(s_prompt))).*fft(center(s_prompt));
p1 = p1(1:length(f_prompt));
p2 = conj(fft(center(s_cal_air_down))).*fft(center(s_cal_air_down));
p2 = p2(1:length(f_cal_air_down));
p3 = conj(fft(center(s_cal_snow_down))).*fft(center(s_cal_snow_down));
p3 = p3(1:length(f_cal_snow_down));
%Loop is to correct only signal
low_edge_prompt = find(f_prompt>lower_band,1,'first');
high_edge_prompt = find(f_prompt<upper_band,1,'last');
low_edge_air = find(f_cal_air_down>lower_band,1,'first');
high_edge_air = find(f_cal_air_down<upper_band,1,'last');
low_edge_snow = find(f_cal_snow_down>lower_band,1,'first');
high_edge_snow = find(f_cal_snow_down<upper_band,1,'last');
%~ for i=1:length(p1)
	%~ if(i<low_edge_prompt)
		%~ p1(i) = 10*log10(p1(i))+20.0; %Just to see the noise at the same levels
	%~ else
		%~ p1(i) = 10*log10(p1(i))+distance_factor;
	%~ endif
%~ endfor
%~ for i=1:length(p2)
	%~ if(i<low_edge_air)
		%~ p2(i) = 10*log10(p2(i))+20.0; %Just to see the noise at the same levels
	%~ else
		%~ p2(i) = 10*log10(p2(i))+atten_3dB;
	%~ endif
%~ endfor
%~ for i=1:length(p3)
	%~ if(i<low_edge_snow)
		%~ p3(i) = 10*log10(p3(i))+20.0; %Just to see the noise at the same levels
	%~ else
		%~ p3(i) = 10*log10(p3(i))+atten_20dB+atten_3dB;
	%~ endif
%~ endfor
p1 = 10*log10(p1)+distance_factor+20;
p2 = 10*log10(p2)+atten_3dB+20.0;
p3 = 10*log10(p3)+atten_20dB+atten_3dB+20.0;
plot(f_prompt,p1,'linewidth',linewc2,'color','black');
axis([0.0 1.0 -60.0 60.0]);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Frequency (GHz)");
ylabel("Power (dB)");
title("(0 dB = DC background)");
plot(f_cal_air_down,p2,'linewidth',linewc2,'color','red');
plot(f_cal_snow_down,p3,'linewidth',linewc2,'color','blue');
L = legend('Surface Prompt Component','Calibration (Air)','Calibration (Snow)','location','south');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linewc2);
set(gca,'tickLength',[0.025 0.025]);

%The power spectrum of the delayed component
%p = conj(fft(s3)).*fft(s3);
%p = p(1:length(f));
%plot(f3,10*log10(p)+20.0,'linewidth',linewc,'color','black');
%axis([0.1 1.0 -1.0 70]);
%set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
%xlabel("Frequency (GHz)");
%ylabel("Power (dB)");
%title("Delayed Component Only. (0 dB = average bkgd.)");

%The saved graph.
print -dpdf 'March9_plot1.pdf'
