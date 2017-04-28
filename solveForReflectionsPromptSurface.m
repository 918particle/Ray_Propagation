pkg load control
pkg load signal
pkg load image

lower_band = 0.0;
upper_band = 1.2;
delta_f = 0.05; %GHz
frequency_bins = linspace(lower_band+delta_f,upper_band,floor((upper_band-lower_band)/delta_f));

prompt_component_start_sample = 9650;
prompt_component_length = 3000;
prompt_downsample = 4;

calibration_start_sample_air = 11900;
calibration_length_air = 3000;
calibration_start_sample_snow = 12050;
calibration_length_snow = 3000;
calibration_downsample = 8;

distance_long = 543.0;
distance_short = 23.0;
distance_factor = 10*log10(distance_long/distance_short);
atten_20dB = 20.0;
atten_3dB = 3.0;
fc_filter = 0.08; %GHz
n_poles = 4;

linew = 2.0;
linew2 = 4.0;
fon = "Courier";
fonts = 20.0;
fonts2 = 14.0;
axisVector = [350.0 450.0 -.1 .1];
axisVector2 = [0 1.0 -50 50];
axisVector3 = [0 0.4 -50 50];

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

%Equilibrate the waveform lengths
s_cal_snow_down = padarray(s_cal_snow_down,length(s_prompt)-length(s_cal_snow_down),"post");
s_cal_air_down = padarray(s_cal_air_down,length(s_prompt)-length(s_cal_air_down),"post");

%Successively add reflections to first pulse until correlation is maximized, examine effect on power spectrum
time_1 = 8;
time_2 = 45;
time_3 = time_2;
time_4 = 100;
time_5 = 100;
time_6 = 141;
time_7 = 141;
time_8 = 191;
time_9 = 191;
time_10 = 267;
r_2 = 1.24;
r_3 = 1.0;
r_4 = 1.3;
r_5 = 1.0;

s_prompt_first = s_prompt(time_1:time_2);
t_prompt_first = t_prompt(time_1:time_2);
s_prompt_second = s_prompt(time_3:time_4);
t_prompt_second = t_prompt(time_3:time_4);
s_prompt_third = s_prompt(time_5:time_6);
t_prompt_third = t_prompt(time_5:time_6);
s_prompt_forth = s_prompt(time_7:time_8);
t_prompt_forth = t_prompt(time_7:time_8);
s_prompt_fifth = s_prompt(time_9:time_10);
t_prompt_fifth = t_prompt(time_9:time_10);
s_prompt_first = padarray(s_prompt_first,time_1,"pre");
s_prompt_first = padarray(s_prompt_first,length(s_prompt)-length(s_prompt_first),"post");
s_prompt_second = padarray(s_prompt_second,time_3,"pre");
s_prompt_second = padarray(s_prompt_second,length(s_prompt)-length(s_prompt_second),"post");
s_prompt_third = padarray(s_prompt_third,time_5,"pre");
s_prompt_third = padarray(s_prompt_third,length(s_prompt)-length(s_prompt_third),"post");
s_prompt_forth = padarray(s_prompt_forth,time_7,"pre");
s_prompt_forth = padarray(s_prompt_forth,length(s_prompt)-length(s_prompt_forth),"post");
s_prompt_fifth = padarray(s_prompt_fifth,time_9,"pre");
s_prompt_fifth = padarray(s_prompt_fifth,length(s_prompt)-length(s_prompt_fifth),"post");

s_add = s_prompt_first+r_2*circshift(s_prompt_second,[-time_3,0])+r_3*circshift(s_prompt_third,[-time_5,0]) ... 
+r_4*circshift(s_prompt_forth,[-time_7,0])+r_5*circshift(s_prompt_fifth,[-time_9,0]);

%~ result = 0.0;
%~ results = [];
%~ for r_2=1.0:0.02:1.3
	%~ for r_3=1.0:0.1:1.3
		%~ for r_4=1.0:0.02:1.3
			%~ for r_5=1.0:0.1:1.3
				%~ display([r_2 r_3 r_4 r_5])
				%~ s_add = s_prompt_first+r_2*circshift(s_prompt_second,[-time_3,0])+r_3*circshift(s_prompt_third,[-time_5,0]) ... 
				%~ +r_4*circshift(s_prompt_forth,[-time_7,0])+r_5*circshift(s_prompt_fifth,[-time_9,0]);
				%~ x=corr(s_prompt(time_1:time_2),s_add(time_1:time_2));
				%~ if(x>result)
					%~ results = [r_2 r_3 r_4 r_5];
					%~ s_add_best = s_add;
					%~ result = x;
				%~ endif
			%~ endfor
		%~ endfor
	%~ endfor
%~ endfor
%~ display(result)
%~ display(results)
[b,a] = butter(n_poles,fc_filter/fc_prompt,'high');
s_add = filter(b,a,s_add);

%Repeat, for in-snow calibration waveform
time_1 = 1;
time_2 = 30;
time_3 = time_2;
time_4 = 53;
time_5 = 58;
time_6 = 90;
time_7 = time_6;
time_8 = 120;

s_snow_first = s_cal_snow_down(time_1:time_2);
t_snow_first = t_cal_snow_down(time_1:time_2);
s_snow_second = s_cal_snow_down(time_3:time_4);
t_snow_second = t_cal_snow_down(time_3:time_4);
s_snow_third = s_cal_snow_down(time_5:time_6);
t_snow_third = t_cal_snow_down(time_5:time_6);
s_snow_forth = s_cal_snow_down(time_7:time_8);
t_snow_forth = t_cal_snow_down(time_7:time_8);
s_snow_first = padarray(s_snow_first,length(s_cal_snow_down)-length(s_snow_first),"post");
s_snow_second = padarray(s_snow_second,time_3-1,"pre");
s_snow_second = padarray(s_snow_second,length(s_cal_snow_down)-length(s_snow_second),"post");
s_snow_third = padarray(s_snow_third,time_5,"pre");
s_snow_third = padarray(s_snow_third,length(s_cal_snow_down)-length(s_snow_third),"post");
s_snow_forth = padarray(s_snow_forth,time_7,"pre");
s_snow_forth = padarray(s_snow_forth,length(s_cal_snow_down)-length(s_snow_forth),"post");
s_snow_add = s_snow_first+circshift(s_snow_second,[-4,0])+circshift(s_snow_third,[-18,0])-circshift(s_snow_forth,[20,0]);

%~ result = 0.0;
%~ results = [];
%~ time_2 = 30;
%~ time_3 = time_2
%~ for time_4=1:100
	%~ time_5 = time_4;
	%~ for time_6=1:100
		%~ display([time_1 time_2 time_3 time_4 time_5 time_6])
		%~ s_snow_first = s_cal_snow_down(time_1:time_2);
		%~ t_snow_first = t_cal_snow_down(time_1:time_2);
		%~ s_snow_second = s_cal_snow_down(time_3:time_4);
		%~ t_snow_second = t_cal_snow_down(time_3:time_4);
		%~ s_snow_third = s_cal_snow_down(time_5:time_6);
		%~ t_snow_third = t_cal_snow_down(time_5:time_6);
		%~ s_snow_first = padarray(s_snow_first,length(s_cal_snow_down)-length(s_snow_first),"post");
		%~ s_snow_second = padarray(s_snow_second,time_3-1,"pre");
		%~ s_snow_second = padarray(s_snow_second,length(s_cal_snow_down)-length(s_snow_second),"post");
		%~ s_snow_third = padarray(s_snow_third,time_5,"pre");
		%~ s_snow_third = padarray(s_snow_third,length(s_cal_snow_down)-length(s_snow_third),"post");
		%~ s_snow_add = s_snow_first+circshift(s_snow_second,[-time_3,0])+circshift(s_snow_third,[-time_5,0]);
		%~ x=corr(s_snow_add,s_snow_first);
		%~ if(x>result)
			%~ s_snow_add_best = s_snow_add;
			%~ result = x;
			%~ results = [time_1 time_2 time_3 time_4 time_5 time_6];
		%~ endif
	%~ endfor
%~ endfor
%~ display(result)
%~ display(results)
%~ [b,a] = butter(n_poles,fc_filter/fc_prompt,'high');
%~ s_snow_add = filter(b,a,s_snow_add_best);

%Create spectrum for the reconstructed, original pulse, and raw data
p1 = conj(fft(center(s_add))).*fft(center(s_add));
p2 = conj(fft(center(s_prompt))).*fft(center(s_prompt));
p3 = conj(fft(center(s_snow_add))).*fft(center(s_snow_add)); %was s_cal_snow_down
p4 = conj(fft(center(s_cal_snow_down))).*fft(center(s_cal_snow_down));
p1 = p1(1:length(f_prompt));
p2 = p2(1:length(f_prompt));
p3 = p3(1:length(f_prompt));
p4 = p4(1:length(f_prompt));
%The 30 dB just sets the DC noise value to be 0 dB
p1 = 10*log10(p1)+distance_factor+25.0;
p2 = 10*log10(p2)+distance_factor+25.0;
p3 = 10*log10(p3)+atten_20dB+25.0;
p4 = 10*log10(p4)+atten_20dB+25.0;
p1_ave = zeros(size(frequency_bins));
p1_err = zeros(size(frequency_bins));
p2_ave = zeros(size(frequency_bins));
p2_err = zeros(size(frequency_bins));
p3_ave = zeros(size(frequency_bins));
p3_err = zeros(size(frequency_bins));
p4_ave = zeros(size(frequency_bins));
p4_err = zeros(size(frequency_bins));
for i=1:(length(frequency_bins)-1)
	j = find(f_prompt>=frequency_bins(i),1,'first');
	k = find(f_prompt>=frequency_bins(i+1),1,'first');
	p1_ave(i) = mean(p1(j:k));
	p2_ave(i) = mean(p2(j:k));
	p3_ave(i) = mean(p3(j:k));
	p4_ave(i) = mean(p4(j:k));
	p1_err(i) = std(p1(j:k))/sqrt(k-j);
	p2_err(i) = std(p2(j:k))/sqrt(k-j);
	p3_err(i) = std(p3(j:k))/sqrt(k-j);
	p4_err(i) = std(p4(j:k))/sqrt(k-j);
endfor
frequency_bins+=delta_f/2.0;
display(frequency_bins)

%Plotting section

%Display each pulse
figure(1);
plot(t_prompt,s_prompt,'color','black','linewidth',linew);
hold on;
plot(t_prompt,s_prompt_first,'-','color','red','linewidth',linew);
plot(t_prompt,s_prompt_second,'-','color','blue','linewidth',linew);
plot(t_prompt,s_prompt_third,'-','color','green','linewidth',linew);
plot(t_prompt,s_prompt_forth,'-','color','magenta','linewidth',linew);
plot(t_prompt,s_prompt_fifth,'-','color','yellow','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
%The saved graph.
print -dpdf 'March28_plot1.pdf'

figure(2);
plot(t_prompt,s_prompt_first,'color','red','linewidth',linew);
hold on;
plot(t_prompt,s_prompt_second,'-','color','blue','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Surface Prompt Initial','Delayed','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot2.pdf'

figure(3);
plot(t_prompt,s_prompt_first,'color','red','linewidth',linew);
hold on;
plot(t_prompt,s_prompt_third,'-','color','green','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Surface Prompt Initial','Delayed','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot3.pdf'

figure(4);
plot(t_prompt,s_prompt_first,'color','red','linewidth',linew);
hold on;
plot(t_prompt,s_prompt_forth,'-','color','magenta','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Surface Prompt Initial','Delayed','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot4.pdf'

figure(5);
plot(t_prompt,s_prompt_first,'color','red','linewidth',linew);
hold on;
plot(t_prompt,s_prompt_fifth,'-','color','yellow','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Surface Prompt Initial','Delayed','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot5.pdf'

figure(6);
hold on;
plot(t_prompt,s_prompt,'color','black');
plot(t_prompt,s_add,'color','blue','linewidth',linew);
axis(axisVector);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Surface Prompt','Corrected for reflections','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot6.pdf'

figure(7);
hold on;
plot(t_prompt,s_cal_snow_down+0.01,'color','black');
plot(t_prompt,s_snow_add,'color','red','linewidth',linew);
%~ plot(t_prompt,s_snow_first,'color','blue','linewidth',linew);
%~ plot(t_prompt,s_snow_second,'color','green','linewidth',linew);
%~ plot(t_prompt,s_snow_third,'color','yellow','linewidth',linew);
axis([350 450 -.05 .05]);
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel("Time (ns)");
ylabel("Voltage/(Max. Voltage)");
set(gca,'tickLength',[0.025 0.025]);
L = legend('Snow Cal.','Corrected for reflections','location','northeast');
FL = findall(L,'-property','linewidth');
set(FL,'linewidth',linew);
set(L,'fontsize',fonts2,'fontname',fon);
%The saved graph.
print -dpdf 'March28_plot7.pdf'

figure(8);
hold on;
axis(axisVector2);
set(gca, "linewidth", linew, "fontname",fon,"fontsize",fonts);
xlabel("Frequency (GHz)");
ylabel("Power (dB) relative");
title("Blue: Cleaned prompt, Black: Raw prompt, Red: In-snow cal.","fontname",fon,"fontsize",fonts*3/4,"fontweight","normal");
herr1 = errorbar(frequency_bins(1:end-1),p1_ave(1:end-1),p1_err(1:end-1),'~o--');
set(herr1,'color','blue');
herr2 = errorbar(frequency_bins(1:end-1),p3_ave(1:end-1),p3_err(1:end-1),'~o--');
set(herr2,'color','red');
herr3 = errorbar(frequency_bins(1:end-1),p2_ave(1:end-1),p2_err(1:end-1),'~o--');
set(herr3,'color','black');
herr4 = errorbar(frequency_bins(1:end-1),p4_ave(1:end-1),p4_err(1:end-1),'~o--');
set(herr4,'color','magenta');
set(gca,'tickLength',[0.025 0.025]);
grid on;
%The saved graph.
print -dpdf 'March28_plot8.pdf'

figure(9);
hold on;
axis(axisVector3);
set(gca, "linewidth", linew, "fontname",fon,"fontsize",fonts);
xlabel("Frequency (GHz)");
ylabel("Power (dB) relative");
title("Blue: Cleaned prompt, Black: Raw prompt, Red: In-snow cal.","fontname",fon,"fontsize",fonts*3/4,"fontweight","normal");
herr1 = errorbar(frequency_bins(1:end-1),p1_ave(1:end-1),p1_err(1:end-1),'~o--');
set(herr1,'color','blue');
herr2 = errorbar(frequency_bins(1:end-1),p3_ave(1:end-1),p3_err(1:end-1),'~o--');
set(herr2,'color','red');
herr3 = errorbar(frequency_bins(1:end-1),p2_ave(1:end-1),p2_err(1:end-1),'~o--');
set(herr3,'color','black');
set(gca,'tickLength',[0.025 0.025]);
grid on;
%The saved graph.
print -dpdf 'March28_plot9.pdf'

%~ for time_4=70:80
	%~ time_5 = 100;
	%~ for time_6=140:145
		%~ time_7 = time_6;
		%~ for time_8=180:200
			%~ time_9=time_8;
			%~ for time_10=245:250
				%~ display([time_1 time_2 time_3 time_4 time_5 time_6 time_7 time_8 time_9 time_10])
				%~ %Divide up the pulse into sub-pulses
				%~ s_prompt_first = s_prompt(time_1:time_2);
				%~ t_prompt_first = t_prompt(time_1:time_2);
				%~ s_prompt_second = s_prompt(time_3:time_4);
				%~ t_prompt_second = t_prompt(time_3:time_4);
				%~ s_prompt_third = s_prompt(time_5:time_6);
				%~ t_prompt_third = t_prompt(time_5:time_6);
				%~ s_prompt_forth = s_prompt(time_7:time_8);
				%~ t_prompt_forth = t_prompt(time_7:time_8);
				%~ s_prompt_fifth = s_prompt(time_9:time_10);
				%~ t_prompt_fifth = t_prompt(time_9:time_10);
				%~ s_prompt_first = padarray(s_prompt_first,time_1,"pre");
				%~ s_prompt_first = padarray(s_prompt_first,length(s_prompt)-length(s_prompt_first),"post");
				%~ s_prompt_second = padarray(s_prompt_second,time_3,"pre");
				%~ s_prompt_second = padarray(s_prompt_second,length(s_prompt)-length(s_prompt_second),"post");
				%~ s_prompt_third = padarray(s_prompt_third,time_5,"pre");
				%~ s_prompt_third = padarray(s_prompt_third,length(s_prompt)-length(s_prompt_third),"post");
				%~ s_prompt_forth = padarray(s_prompt_forth,time_7,"pre");
				%~ s_prompt_forth = padarray(s_prompt_forth,length(s_prompt)-length(s_prompt_forth),"post");
				%~ s_prompt_fifth = padarray(s_prompt_fifth,time_9,"pre");
				%~ s_prompt_fifth = padarray(s_prompt_fifth,length(s_prompt)-length(s_prompt_fifth),"post");
				%~ s_add = s_prompt_first+circshift(s_prompt_second,[-time_3,0])+circshift(s_prompt_third,[-time_5,0]) ... 
				%~ +circshift(s_prompt_forth,[-time_7,0])+circshift(s_prompt_fifth,[-time_9,0]);
				%~ x = corr(s_prompt(time_1:time_2),s_add(time_1:time_2));
				%~ if(x>result)
					%~ result = x;
					%~ results = [time_1 time_2 time_3 time_4 time_5 time_6 time_7 time_8 time_9 time_10];
					%~ s_add_best = s_add;
				%~ endif
			%~ endfor
		%~ endfor
	%~ endfor
%~ endfor
