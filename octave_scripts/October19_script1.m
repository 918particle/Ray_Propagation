pkg load signal

distance_long = 543.0;
distance_short = 23.0;
distance_factor = 10*log10(distance_long/distance_short);

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
figure(1,'Position',[0,0,800,800]);
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