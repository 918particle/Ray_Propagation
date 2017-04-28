function RayProp_jch

c0 = 299792458; %Speed of light, m/s
%~ f = @nfirnmeas_SPICE; %Function handle for firn from SPICE Core
%~ f = @nfirnmeas1; %Function handle for firn measurement #1
%~ f = @nfirn;
f = @nfirn_sp;
zmax=0; %Maximum depth of the medium
zmin=-1500; %Minimum depth of the medium
z0 = -1400; %Position of the emitter in z coordinate
x0 = 0.0; %Position of the emitter in x coordinate
reflection = 1; %Account for reflections?
dt = 1/c0; %time steps, 1 meter divided by speed of light in m/s
tend = 10000.0/c0; %end time, n time steps
dangle=0.4; %angular step, in degrees
angrange = 10:dangle:25; %angular range
rx_extent = 0.5; %sqrt(area) of receiver

% Position of receivers:
rx = [4000.0 -0.0 -200.0];
% find receiver that is furthest away
xmaxrx = max(rx(:,1));

% calculate angle w.r.t. to position of emitter
if ~isempty(rx)
    for ii=1:size(rx,1)
        vec1 = [ (rx(ii,1)-x0) (rx(ii,2)-z0) ];
        vec2 = [ (rx(ii,1)-x0) (rx(ii,3)-z0) ];
        cosvec1 = dot(vec1,[1 0])/norm(vec1);
        cosvec2 = dot(vec2,[1 0])/norm(vec2);
        cosang = dot(vec1 , vec2 );
        cosang = cosang/(norm(vec1)*norm(vec2));
        rxang(ii) = acos(cosang);
        rxsinvec1(ii) = dot(vec1,[0 1])/norm(vec1);
        asin(rxsinvec1(ii))
        rxsinvec2(ii) = dot(vec2,[0 1])/norm(vec2);
        asin(rxsinvec2(ii))
        rxcos(ii) = 0.5*(dot(vec1,[1 0])/norm(vec1) + dot(vec2,[1 0])/norm(vec2));
        rxdist(ii) = norm((vec1+vec2)./2);
        rxlen(ii) = norm(vec2-vec1);
    end
end

figure(1,'Position',[0,0,1200,240]);
linew = 3.0;
fon = "Courier";
fonts = 20.0;
linewc = 0.0;
linewc2 = 4.0;
hold on;
set(gca, "linewidth", linew, "fontname", fon,"fontsize",fonts);
xlabel('Distance (m)');
ylabel('Depth (m)');
title("Smooth Index of Refraction Model","fontname", fon,"fontsize",fonts);
ndraw=max(1,round(0.1/dangle));
idraw = 0;
for angle = angrange;
	idraw=idraw+1;
    z = z0;
    x = x0;
    alpha = angle/180*pi;
    t = 0;
    goon = 1;
    tarray = [];
    alpharray = [];
    narray = [];
    xarray = [];
    zarray = [];
    while goon        
        tarray = [tarray; t];
        xarray = [xarray; x];
        zarray = [zarray; z];
        n = f(z);
        zold = z;
        xold =x;
        z = z + sin(alpha)*dt*c0/(n);
        x = x + cos(alpha)*dt*c0/(n);
        if (z>=0 && reflection),
            alpha = -alpha;
            z = 0;
        end
        if (abs(z-zold) > 1e-4)
            dndz = (f(z)-f(zold))/(z-zold);
        else
            dndz = (f(z)-f(z-1e-4))/(z-1e-4);
        end
        dcdz = -dndz*c0/(n*n);
        dalpha = -dt*cos(alpha)*dcdz;
        alpha = alpha+dalpha;
        alpharray = [alpharray; alpha];
        narray = [narray; n];        
        t = t + dt;
        if (t>tend), goon = 0; end
        if (z>zmax), goon = 0; end
        if (z<zmin), goon = 0; end
        if (x>xmaxrx), goon = 0; end
    end
    xarray = [xarray; x];
    zarray = [zarray; z];
    if(idraw==ndraw)
        plot (xarray,zarray,'-','LineWidth',1,'Color','b','Linestyle','-')
        idraw = 0;
    end
end
if ~isempty(rx)
    for ii=1:size(rx,1)
        h3 = line([rx(ii,1), rx(ii,1)],[rx(ii,3)+rx_extent, rx(ii,3)-rx_extent]);
        set(h3,'Color','r','LineStyle','-','LineWidth',3);
    end
end
%The saved graph.
print -dpdf 'April28_plot6.pdf'
end

function y = nfirn(z)
y = 1.0 + 0.86*(1.0 - 0.638 * exp(z/34.7) );
end

function y = nfirn_sp(z)
y = 1.78-0.427*exp(z/71.0);
end

function y = nfirnmeas1(z)
vdepth= [0 0.5 1 1.5 2 3 3.8 4.2 4.8 5.6 6.8 7.7 8 8.8 9.7 10.3 10.7 11.8 12.4 12.7 13.8 14.7 15.8 16.4 16.8 17.9 18.4 18.7 19.9 20.5 ];
vdepth = vdepth*(-1);
vrho = [ 329.9 329.9 329.7 349.2 408.8 382.2 406.7 463.3 406.4 439.2 447.0 ...
    445.9 443.8 483.9 472.3 506.4 486.8 493.1 523.8 518.4 517.7 525.8 553.0 545.0 549.8 568.7 584.3 537.0 563.0 568.5];
if z<=0,
    itemp = find (z<=vdepth);
    if(length(itemp)>=length(vdepth))
		y = nfirn(z);
	else
		idepth = itemp(end);
		rho = vrho(idepth) + (vrho(idepth+1) - vrho(idepth))/(vdepth(idepth+1) - vdepth(idepth))*(z - vdepth(idepth));
		rho=rho/1000; %scale to density of water
		y = 1 + 0.86*rho;
	end
else
    y = 1;
end
end

function y = nfirnmeas_SPICE(z)
data = load('/home/jordan/ANewHope/SPICE_data.dat');
vdepth = -data(:,1);
vrho = data(:,2);
if z<=0,
    itemp = find(z<=vdepth);
    if(length(itemp)>=length(vdepth))
		y = nfirn_sp(z);
	else
		idepth = itemp(end);
		rho = vrho(idepth) + (vrho(idepth+1) - vrho(idepth))/(vdepth(idepth+1) - vdepth(idepth))*(z - vdepth(idepth));
		rho=rho/1000; %scale to density of water
		y = 1 + 0.86*rho;
	end
else
    y = 1;
end
end
