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

s = filter(b,a,s);
s = filter(d,c,s);
s = s+randn(size(s))*max(s)/1.0e3;

s = s-mean(s);
s = s/max(s);
[S,F,T] = specgram(s,n,fs,hanning(n),m);
T = T+t(1);
T = T/1.0e3; %Units change: microseconds
Sdb = 10.0*log10(abs(S));
St = sum(S,1);
Sf = 10*log10(sum(abs(S),2))-max(10*log10(sum(abs(S),2)));

s1 = s;
t = t-t(1);
s1(find(t>0.4))=0.0;
s1(find(t<0.2))=0.0;
[S1,F1,T1] = specgram(s1,n,fs,hanning(n),m);
T1 = T1/1.0e3; %Units change: microseconds
T1 = T1+t(1);
Sf1 = 10*log10(sum(abs(S1),2))-max(10*log10(sum(abs(S),2)))+10*log10(1.75/(0.4-0.2));

s2 = s;
t = t-t(1);
s2(find(t>0.6))=0.0;
s2(find(t<0.4))=0.0;
[S2,F2,T2] = specgram(s2,n,fs,hanning(n),m);
T2 = T2/1.0e3; %Units change: microseconds
T2 = T2+t(1);
Sf2 = 10*log10(sum(abs(S2),2))-max(10*log10(sum(abs(S),2)))+10*log10(1.75/(0.6-0.4));

figure(1,'Position',[0,0,1600,1600]);
subplot(2,2,2);
grid off;
box on;
colormap(hot(15));
contourf(T,F,Sdb,[-30:3:6],'linewidth',linewc,"linecolor",linec);
colorbar;
axisVector = [0 1.75 -0.01 1.5];
axis(axisVector);
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
set(findobj(gcf(),'tag','colorbar'),"linewidth",linew,"fontname",fon,"fontsize",fonts);
set(ylabel("Frequency (GHz)"),'position',[-0.4,0.7,0]);
set(xlabel("Time (microseconds)"),'position',[0.9 -0.17 0]);
title("dB (Max Voltage)");
caxis([-30 6]);
subplot(2,2,1);
grid off;
box on;
hold on;
plot(Sf,F,"color",linec,"linewidth",linewl);
plot(Sf1,F1,"color","red","linewidth",linewl);
plot(Sf2,F2,"color","blue","linewidth",linewl);
[F2' Sf2]
axisVector = [-30 6 -0.01 1.5];
axis(axisVector);
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
set(findobj(gcf(),'tag','colorbar'),"linewidth",linew,"fontname",fon,"fontsize",fonts);
set(ylabel("Frequency (GHz)"),'position',[-39.75,0.7,0]);
set(xlabel("dB (Max Voltage)"),'position',[-15 -0.17 0]);
subplot(2,2,4);
plot(downsample(t,10),downsample(s1,10),"color",linec,"linewidth",linewl);

%The saved graph.
print -dpng -r300 'October20_plot1.png'