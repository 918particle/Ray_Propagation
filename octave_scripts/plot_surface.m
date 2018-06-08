pkg load signal
pkg load optim
close all 
graphics_toolkit fltk

distance_long = 543.0;
distance_short = 23.0;

nl = -30.0;
delta_dB = 3;
n = 1024; 
m = 0;
linew = 2.0;
linew2 = 0.75;
mark_size = 7;
fon = "Courier";
fonts = 20.0;
axisVector_contour = [nl:delta_dB:0];
Fmax = 1.0; %Units: GHz
power_level = -20;
tolerance = 3.0;
window_time = 10.0; %nanoseconds

%Surface propagation data
fc1 = 0.08;
data = load('surface_propagation.dat');
t = data(:,1)*1.0e9; %Units: nanoseconds
s = data(:,2);
s = s-mean(s);
%How many samples is window_time nanoseconds?
window_sh = floor(10.0/(t(2)-t(1)));

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
dT = T(2)-T(1);
dF = F(2)-F(1);
Sdb = 10.0*log10(abs(S));
Sdb = Sdb - max(max(Sdb));
Sdb(find(Sdb<=nl)) = nl;
axisVector1 = [0 T(end) 0.0 Fmax];
axisVector2 = [0.0 Fmax nl 0.0];
axisVector3 = [0 T(end) 0.0 Fmax];
axisVector4 = [0 t(end) 0.0 1.0];
[x,y] = size(Sdb);
F_low = find(F>0.4,1);
T_high = length(T);
T_low = find(T>0.28,1);
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

chirp_data_err_x = ones(size(chirp_data(:,1)))*dT/2;
chirp_data_err_y = ones(size(chirp_data(:,1)))*dF/2;
t_off_0 = T(T_low);
t_off_1 = T(T_low+2);

%The offset time should be fixed to T_low.
function sum_square_errors = model(p,x,y,t_off)
	y_trial = p(1) + p(2)*exp(-(x-t_off)/p(3));
	sum_square_errors = sum((y-y_trial).^2);
endfunction

fun = @(p) model(p,chirp_data(:,1),chirp_data(:,2),t_off_0);
initial_params = [0.175,0.1,3];
params = fminsearch(fun,initial_params);
x0 = linspace(T(1),T(end),100);
y0 = params(1) + params(2)*exp(-(x0-0.36750)/params(3));
params
%Evaluate chi-squared:
evaluated = params(1)+params(2)*exp(-(chirp_data(:,1)-0.36750)/params(3));
numerator = chirp_data(:,2)-evaluated;
chi_squared = sum(numerator.^2./chirp_data_err_y.^2)/length(evaluated);
chi_squared

%Hilbert Transform to measure arrival times accurately
sh = abs(hilbert(s));
sh_filter = filter(ones(window_sh,1)/window_sh,1,sh);
sh_filter = sh_filter/max(sh_filter);
[~,imax] = max(sh_filter);
sh_filter_rotate = sh_filter;
i = find(t>t_off_1,1);
sh_filter_rotate(i:end) = 0.0;
corr_data = [];
for i=1:length(sh_filter)
	this_sh_filter = circshift(sh_filter_rotate,[i,0]);
	corr_data = [corr_data; [(t(2)-t(1))*i+t(imax) corr(sh_filter,this_sh_filter)]];
endfor

%Plotting
%h1 = figure(1,'Position',[0,0,1600,1600]);
%grid off;
%box on;
%hold on;
%colormap(bone(length(axisVector_contour)-1));
%[C,h] = contourf(T,F,Sdb,axisVector_contour);
%plot(chirp_data(:,1),chirp_data(:,2),'.','color','red','markersize',mark_size);
%set(h,'LineColor','none');
%cb = colorbar("eastoutside");
%%caxis([min(axisVector_contour) max(axisVector_contour)]);
%set(cb,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ytick",axisVector_contour);
%set(gca(),'xtick',[0:0.5:T(end)]);
%axis(axisVector1);
%axis("square");
%set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
%ylabel("Frequency (GHz)");
%xlabel("Time (microseconds)",'position',[0.9 -0.06 0]);
%title("dB (relative to max)");
%The saved graph.
%print -dpng 'June7_plot1.png'

%figure(2,'Position',[0,0,1600,1600]);
%grid off;
%box on;
%hold on;
%plot(F,Sdb(:,T_low),'-','color',[0 0 0],'linewidth',linew);
%plot(F,Sdb(:,T_low+3),'--','color',[0.4 0.4 0.4],'linewidth',linew);
%plot(F,Sdb(:,T_low+15),'--','color',[0.8 0.8 0.8],'linewidth',linew);
%axis(axisVector2);
%axis("square");
%set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
%xlabel("Frequency (GHz)");
%ylabel("dB (relative to max)");
%The saved graph.
%print -dpng 'June7_plot2.png'

%figure(3,'Position',[0,0,1600,1600]);
%hold on;
%grid off;
%box on;
%h1 = errorbar(chirp_data(:,1),chirp_data(:,2),chirp_data_err_x,chirp_data_err_y,'~>');
%set(h1,'linestyle','none','marker','.','markersize',mark_size,'color','red');
%plot(x0,y0,'--','linewidth',linew2,'color','black');
%set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
%axis(axisVector3);
%axis("square");
%ylabel("Frequency (GHz)",'position',[-0.3 0.5 0]);
%xlabel("Time (microseconds)");
%%The saved graph.
%print -dpng 'June7_plot3.png'

figure(4,'Position',[0,0,1600,1600]);
grid off;
box on;
hold on;
plot(downsample(t,5),downsample(sh_filter,5),'-','color','black');
plot(corr_data(1500:end,1),abs(corr_data(1500:end,2)));
axis(axisVector4);
axis("square");
set(gca,"linewidth",linew,"fontname",fon,"fontsize",fonts,"ticklength",[0.05 0.05]);
xlabel("Time (microseconds)");
ylabel("Normalized Amplitude");
%The saved graph.
print -dpng 'June7_plot4.png'