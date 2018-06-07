pkg load signal
close all 
graphics_toolkit fltk 

distance_long = 543.0;
distance_short = 23.0;

nl = -30.0;
delta_dB = 1;
n = 800; 
m = 80;
linew = 2.0;
fon = "Courier";
fonts = 13.0;
axisVector_contour = [nl:delta_dB:0];
Fmax = 1.0; %Units: GHz
start_slice = 13;
power_level = -30:-15.0;

%Surface propagation data
fc1 = 0.08;
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
s = s-mean(s);

dt = t(2)-t(1);
fs = 1.0/dt; %Units: GHz
t = t/1.0e3; %Units: microseconds
[b,a] = butter(2,fc1/(fs/2),'high');
%Filtering
s = filter(b,a,s);
clear a b c d;
%Fourier and Time Domains
[S,F,T] = specgram(s,n,fs,hanning(n),m);
T = T/1.0e3; %Units change: microseconds
T = T+t(1);
Sdb = 10.0*log10(abs(S));
Sdb = Sdb - max(max(Sdb));
Sdb(find(Sdb<=nl)) = nl;
axisVector1 = [0 T(end) 0.0 Fmax];
axisVector2 = [nl 0.0 0.0 Fmax];
[x,y] = size(Sdb);
F_low = find(F>0.4,1);
T_high = find(T>1.45,1);
T_low = find(T>0.33,1);
chirp_data = [];

for this_power_level=power_level
	for i=T_low+1:T_high
		%For a given time-slice, we want to know where is the spectrum closest to a give power level?
		mark = 100;
		for j=1:F_low
			if(abs(Sdb(j,i)-power_level)<=mark)
				mark = abs(Sdb(j,i)-this_power_level);
				F_mark = F(j);
			endif
		endfor
		if(F_mark>0 && F_mark<0.25)
			chirp_data = [chirp_data; [T(i) F_mark]];
		endif
	endfor
endfor

p = polyfit(chirp_data(:,1),chirp_data(:,2),2);
x0 = linspace(T(1),T(end),100);
y0 = polyval(p,x0); 

%Plotting
figure(1,'Position',[0,0,1600,1600]);
subplot(2,2,1);
grid off;
box on;
hold on;
colormap(bone(length(axisVector_contour)-1));
[C,h] = contourf(T,F,Sdb,axisVector_contour);
plot(chirp_data(:,1),chirp_data(:,2),'o','color','red','markersize',1);
set(h,'LineColor','none');
colorbar("eastoutside");
caxis([min(axisVector_contour) max(axisVector_contour)]);
set(findobj(gcf(),'tag','colorbar'),
	"linewidth",linew,"fontname",fon,"fontsize",fonts,"xtick",axisVector_contour);
set(gca(),'xtick',[0:0.5:T(end)]);
axis(axisVector1);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
set(ylabel("Frequency (GHz)"),'position',[-0.45,0.5,0]);
set(xlabel("Time (microseconds)"),'position',[0.9 -0.15 0]);
title("dB (relative to max)");
subplot(2,2,2);
grid off;
box on;
hold on;
plot(Sdb(:,T_low),F,'color',[1 0 0],'linewidth',linew);
for i=T_low+1:T_high
	plot(Sdb(:,i),F,'color',[i/y i/y i/y],'linewidth',linew);
endfor
axis(axisVector2);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05],'YaxisLocation','right');
set(ylabel("Frequency (GHz)"),'position',[6,0.5,0]);
set(xlabel("dB (relative to max)"),'position',[-14.5 -0.15 0]);
subplot(2,2,4);
hold on;
grid off;
box on;
plot(chirp_data(:,1),chirp_data(:,2),'o');
plot(x0,y0,'-');
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05],'YaxisLocation','right');
axis(axisVector1);
axis("square");
set(ylabel("Frequency (GHz)"),'position',[-0.45,0.5,0]);
set(xlabel("Time (microseconds)"),'position',[0.9 -0.15 0]);
%The saved graph.
print -dpng 'October20_plot1.png'