pkg load signal
close all 
graphics_toolkit fltk 

distance_long = 543.0;
distance_short = 23.0;
distance_factor = 10*log10(distance_long/distance_short);

atten_20dB = 20.0;
atten_3dB = 3.0;
ampl_62dB = -62.5;

n = 800; 
m = 0;
linew = 2.0;
fon = "Courier";
fonts = 14.0;
linewc = 0.1;
linewl = 2.0;
linec = "black";

time_bins = [0.4:0.1:2.0];
correction_factor = 22.0;  %Can we explain this number? It is related to
%noise power removed by selecting only signal in a given time-window.

%Surface propagation data
fc1 = 0.08;
fc2 = 1.08;
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
dt = t(2)-t(1);
fs = 1.0/dt; %Units: GHz
t = t/1.0e3; %Units: microseconds
[b,a] = butter(2,fc1/(fs/2),'high');
[d,c] = butter(8,fc2/(fs/2),'low');
%Filtering
s = filter(b,a,s);
s = filter(d,c,s);
clear a;
clear b;
clear c;
clear d;
s_noise = randn(size(s))*max(s)/1.0e3;
s = s+s_noise;
%Fourier and Time Domains
s = s-mean(s);
s = s/max(s);
[S,F,T] = specgram(s,n,fs,hanning(n),m);
T = T+t(1);
T = T/1.0e3; %Units change: microseconds
Sdb = 10.0*log10(abs(S));
SpecMax = max(10*log10(sum(abs(S),2)));
TimeFactor = 10*log10((0.4-0.2)/1.75);
Sf = 10*log10(sum(abs(S),2))-SpecMax;
%Hilbert envelope
sh = abs(hilbert(s));
%RMS and Mean of First Spectral Slice
Sf_mean = 0.0;
Sf_rms = 0.0;

Sf_n = [];
for i=2:(length(time_bins))
	si = s;
	si_noise = s_noise;
	t = t-t(1);
	si(find(t>time_bins(i) | t<time_bins(i-1)))=0.0;
	si_noise(find(t<time_bins(i) & t>time_bins(i-1)))=0.0;
	si_noise*=correction_factor;
	si+=si_noise;
	[Si,Fi,Ti] = specgram(si,n,fs,hanning(n),m);
	Ti/=1.0e3; %Units change: microseconds
	Ti+=t(1);
	Sfi = 10*log10(sum(abs(Si),2))-SpecMax;
	Sf_n = [Sf_n Sfi];
	if(i==2)
		Sf_mean = mean(Sfi);
		Sf_rms = std(Sfi);
	endif
endfor

%Plotting
a=14;
b=14;
c=14;
d=14;

figure(1,'Position',[0,0,1600,1600]);
subplot(2,2,2);
grid off;
box on;
colormap(hot(15));
contourf(T,F,Sdb,[-30:3:6],'linewidth',linewc,"linecolor",linec);
colorbar("location","north");
axisVector = [0 2.0 -0.01 1.5];
axis(axisVector);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
set(findobj(gcf(),'tag','colorbar'),"linewidth",linew,"fontname",fon,"fontsize",fonts);
set(ylabel("Frequency (GHz)"),'position',[-0.5,0.7,0]);
set(xlabel("Time (microseconds)"),'position',[0.9 -0.17 0]);
title("dB (Max Voltage)");
caxis([-30 6]);
subplot(2,2,1);
grid off;
box on;
hold on;
plot(Sf,F,"color",linec,"linewidth",linewl);
for i=1:(length(time_bins)-1)
	cn = i/length(time_bins)+0.05;
	plot(Sf_n(:,i),F,"color",[cn cn cn],"linewidth",linewl);
endfor
plot([Sf_mean Sf_mean],[axisVector(3) axisVector(4)],'--','Color','Red');
plot([Sf_mean+5*Sf_rms Sf_mean+5*Sf_rms],[axisVector(3) axisVector(4)],'--','Color','Red');
plot([Sf_mean-5*Sf_rms Sf_mean-5*Sf_rms],[axisVector(3) axisVector(4)],'--','Color','Red');
axisVector = [-30 6 -0.01 1.5];
axis(axisVector);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
set(findobj(gcf(),'tag','colorbar'),"linewidth",linew,"fontname",fon,"fontsize",fonts);
set(ylabel("Frequency (GHz)"),'position',[-37,0.75,0]);
set(xlabel("dB (Max Voltage)"),'position',[-15 -0.17 0]);
subplot(2,2,4);
grid off;
box on;
hold on;
plot(downsample(t,75),downsample(sh,75),"color",linec,"linewidth",linewl);
%plot(downsample(t,100),downsample(s_noise,100),"color",linec,"linewidth",linewl);
%plot(downsample(t,100),downsample(s1_noise,100),"color","red","linewidth",linewl);

%The saved graph.
print -dpdf 'October20_plot1.pdf'
