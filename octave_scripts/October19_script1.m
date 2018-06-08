pkg load signal
pkg load optim
close all 
graphics_toolkit fltk

distance_long = 543.0;
distance_short = 23.0;

nl = -30.0;
delta_dB = 5;
n = 800; 
m = 0;
linew = 2.0;
linew2 = 0.75;
mark_size = 3.5;
fon = "Courier";
fonts = 14.0;
axisVector_contour = [nl:delta_dB:0];
Fmax = 1.0; %Units: GHz
start_slice = 13;
power_level = -20;
tolerance = 3.0;

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
axisVector3 = [0 T(end) 0.0 Fmax];
[x,y] = size(Sdb);
F_low = find(F>0.4,1);
T_high = find(T>1.7,1);
T_low = find(T>0.33,1);
chirp_data = [];

for i=T_low+1:T_high
	mark = 100;
	for j=1:F_low
		if(abs(Sdb(j,i)-power_level)<=mark)
			mark = abs(Sdb(j,i)-power_level);
			F_mark = F(j);
		endif
		if(mark<tolerance)
			break;
		endif
	endfor
	chirp_data = [chirp_data; [T(i) F_mark]];
endfor

%The offset time should be fixed to T_low.
function sum_square_errors = model(p,x,y)
	y_trial = p(1) + p(2)*exp(-(x-0.36750)/p(3));
	sum_square_errors = sum((y-y_trial).^2);
endfunction

fun = @(p) model(p,chirp_data(:,1),chirp_data(:,2));
initial_params = [0.175,0.1,3];
params = fminsearch(fun,initial_params);
x0 = linspace(T(1),T(end),100);
y0 = params(1) + params(2)*exp(-(x0-0.36750)/params(3));
params

%Plotting
figure(1,'Position',[0,0,1600,1600]);
grid off;
box on;
hold on;
colormap(bone(length(axisVector_contour)-1));
[C,h] = contourf(T,F,Sdb,axisVector_contour);
plot(chirp_data(:,1),chirp_data(:,2),'.','color','red','markersize',mark_size);
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
figure(2,'Position',[0,0,1600,1600]);
grid off;
box on;
hold on;
plot(Sdb(:,T_low),F,'-','color',[0 0 0],'linewidth',linew);
plot(Sdb(:,T_low+2),F,'--','color',[0.4 0.4 0.4],'linewidth',linew);
plot(Sdb(:,T_low+12),F,'--','color',[0.8 0.8 0.8],'linewidth',linew);
axis(axisVector2);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05],'YaxisLocation','right');
set(ylabel("Frequency (GHz)"),'position',[6,0.5,0]);
set(xlabel("dB (relative to max)"),'position',[-14.5 -0.15 0]);
figure(3,'Position',[0,0,1600,1600]);
hold on;
grid off;
box on;
plot(chirp_data(:,1),chirp_data(:,2),'.','markersize',mark_size,'color',[1 0 0]);
plot(x0,y0,'--','linewidth',linew2,'color','black');
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05],'YaxisLocation','right');
axis(axisVector3);
axis("square");
set(ylabel("Frequency (GHz)"),'position',[-0.45,0.5,0]);
set(xlabel("Time (microseconds)"),'position',[0.9 -0.15 0]);
%The saved graph.
print -dpng 'October20_plot1.png'