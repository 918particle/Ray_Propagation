function RayProp

% Speed of light in vacuum in m/s
c0 = 299792458;

%======================================
% individual data for simulation

% firn model
%f = @nfirn;
f = @nfirnmeas1;

% Range of validity of medium
zmax=0;
zmin=-20;

% Position of emitter
z0 = -10.; % start point of simulation in m
x0 = 0;

% reflection on/off (=1/0)
reflection = 1;

% time steps and end time
dt = 1/c0;
tend = 200/c0;

% angular steps in degrees
dangle=0.01;

% range to step through
angrange = 10:dangle:40;

% Position of receivers:
% x, zupper, zlower
%rx = [
%      40 -10.0 -10.6;
%      50 -10.0 -10.6;
%      60 -10.0 -10.6;
%      70 -10.0 -10.6;
%      ];

rx = [
    40 -0.0 -0.5;
    50 -0.0 -0.5;
    60 -0.0 -0.5;
    70 -0.0 -0.5;
    ];

%======================================

% find receiver that is furthest away - ray tracing can be stopped when
% this x-position is reached!
xmaxrx = max(rx(:,1)); 

% initialize quantities to calculate power in receivers
hitcount  = zeros(1,size(rx,1));
powsum1   = zeros(1,size(rx,1));
powsum2   = zeros(1,size(rx,1));
rxdist    = zeros(1,size(rx,1)); % distance of antenna (central point) to source
rxang     = zeros(1,size(rx,1)); % angular extend of antenna w.r.t. source
rxlen     = zeros(1,size(rx,1)); % length in z of antenna
rxsindiff = zeros(1,size(rx,1));
rxcos     = zeros(1,size(rx,1));

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
        %rxsindiff(ii) = abs(sinvec2 - sinvec1);
        %rxsindiff(ii) = abs(sin(acos(cosvec2)) - sin(acos(cosvec1)) );
        rxcos(ii) = 0.5*(dot(vec1,[1 0])/norm(vec1) + dot(vec2,[1 0])/norm(vec2));
        rxdist(ii) = norm((vec1+vec2)./2);
        rxlen(ii) = norm(vec2-vec1);
    end
end


% draw z-profile of index of refraction
figure(2)
clf(2)
hold on
% firn region
for z = zmax:-0.1:zmin;
    plot(z,f(z),'b+')
end
xlabel('depth')
ylabel('index of refraction')


figure(1);
clf(1);
hold on;
xlabel('distance in m');
ylabel('depth in m');


h1 = line([0, 150],[zmax, zmax]);
set(h1,'Color','r','LineStyle',':');
h2 = line([0, 150],[zmin, zmin]);
set(h2,'Color','r','LineStyle',':');

% don't draw all lines that are simulated to calculate the power!
ndraw=max(1,round(0.1/dangle));
idraw = 0;
for angle = angrange;%-15:dangle:15;%18.185:0.00001:18.186;%15:0.02:17;
    
       
    idraw=idraw+1;
    z= z0;
    x= x0;
    
    alpha = angle/180*pi;
    
    t = 0;
    goon = 1;
    
    tarray = [];
    alpharray = [];
    narray = [];
    
    xarray = [];
    zarray = [];
    
    while goon
        %for t = 0:dt:tend
        
        tarray(end+1) = t;
        
        n = f(z);
        
        xarray(end+1) = x;
        zarray(end+1) = z;
        
        zold = z; xold =x;
        z = z + sin(alpha)*dt*c0/(n);
        x = x + cos(alpha)*dt*c0/(n);
        
        % reflection
        if (z>=0 && reflection),
            %disp('Reached surface!!!')
            alpha = -alpha;
            z = 0;
        end
        
        % did ray hit receiver?
        for ii=1:size(rx,1)
            % power for cylinder surface
            % proper range of x?
            if (xold<rx(ii,1) && x>=rx(ii,1))
                % proper range of z? Look at interpolated z-postion at
                % x-postion of antenna
                
                dzdx = (z-zold)/(x-xold);
                zrx  = zold+dzdx*(rx(ii,1)-xold);
                
                if (zrx<=rx(ii,2) && zrx>rx(ii,3))
                    % ray hit a reveiver!
                    %disp('Hit receiver, angle:')
                    %disp(ii)
                    %disp(angle)
                    %rxvec = [rx(ii,1) zrx] - [x0 z0] ;
                    %cosrxvec = dot(rxvec,[x-xold z-zold])/(norm(rxvec)*norm([x-xold z-zold]));
                    
                    powsum1(ii) = powsum1(ii)+(dangle/180*pi)*cos(angle/180*pi)/(rxlen(ii)*rx(ii,1));
                    
                end
            end
            % power for sphere
            % proper distance from source?
            if (((xold-x0)*(xold-x0)+(zold-z0)*(zold-z0))<rxdist(ii)*rxdist(ii) && ((x-x0)*(x-x0)+(z-z0)*(z-z0))>=rxdist(ii)*rxdist(ii))
                % proper angle?
                % find intersection point of line from (xold,zold) to (x,z)
                % with sphere
                
                rold = norm([xold-x0 zold-z0]);
                cosrvec = dot([xold-x0 zold-z0],[x-xold z-zold])/(rold*norm([x-xold z-zold]));
                
                pp = 2*rold*cosrvec;
                qq = (rold*rold)-(rxdist(ii)*rxdist(ii));
                lr = -pp/2 + sqrt(pp*pp/4 - qq);
                xr = xold+cos(alpha)*lr;
                zr = zold+sin(alpha)*lr;
                %disp(sqrt((xr-x0)*(xr-x0)+(zr-z0)*(zr-z0)));
                %[xr, zr] is the point on the sphere; to see if it is
                %within the correct angular range, it is sufficient to test
                %for z;
                rang = acos(dot([xr-x0 zr-z0],[1 0])/norm([xr-x0 zr-z0]));
                % resolve ambiguity between angles above and below emitter!
                if (zr<z0),rang=2*pi-rang,end
                
                % if receiver below emitter, sign of angle of receivers is
                % wrong !!??
                if (rang<=asin(rxsinvec1(ii)) && rang>asin(rxsinvec2(ii)))
                    hitcount(ii) = hitcount(ii) + 1;
                   
                    %anglesindiff = sin((angle+dangle)/180*pi)-sin(angle/180*pi);
               
                    %powsum2(ii) = powsum2(ii)+anglesindiff/(rxdist(ii)*rxdist(ii)*rxsindiff(ii));
                    powsum2(ii) = powsum2(ii)+(dangle/180*pi)*cos(angle/180*pi)/(rxdist(ii)*rxdist(ii)*abs(rxsinvec1(ii)-rxsinvec2(ii)));
                    %powsum2(ii) = powsum2(ii)+(dangle/180*pi)*cos(angle/180*pi)/(rxdist(ii)*rxdist(ii)*rxcos(ii)*rxang(ii));
                    if (idraw==ndraw || hitcount(ii)==1)
                        plot(xr,zr,'g*')
                        %plot(x,z,'k*')
                        %plot(xold,zold,'r*')
                    end
                end
                
            end
        end
        
        % n = ndepth(idepth) + (ndepth(idepth+1) - ndepth(idepth))/(vdepth(idepth+1) - vdepth(idepth))*(z - vdepth(idepth));
        
        % get dn/dz (gradient of index of refraction with z) from numerical
        % differentiation
        % protect against dz = 0; this can probably be done in a more
        % clever fashion!
        if (abs(z-zold) > 1e-4)
            dndz = (f(z)-f(zold))/(z-zold);
        else
            dndz = (f(z)-f(z-1e-4))/(z-1e-4);
        end
        
        dcdz = -dndz*c0/(n*n);
        
        dalpha = -dt*cos(alpha)*dcdz;
        
        alpha = alpha+dalpha;
        
        alpharray(end+1) = alpha;
        narray(end+1) = n;
        
        t = t + dt;
        if (t>tend),   goon = 0; end
        if (z>zmax),   goon = 0; end
        if (z<zmin),   goon = 0; end
        if (x>xmaxrx), goon = 0; end
    end
    
    xarray(end+1) = x;
    zarray(end+1) = z;
    figure(1);
    if (idraw==ndraw)
        plot (xarray,zarray,'-','LineWidth',1,'Color','b','Linestyle','-')
        idraw = 0;
    end
    
end

% power in receivers

if ~isempty(rx)
    fprintf('Ant. p_rec  rx.ang.ext.  rx_dist \n' )
    for ii=1:size(rx,1)
        fprintf('%i %8.4f %8.4f %12.4f \n',ii,powsum1(ii),rxang(ii),rxdist(ii) )
    end
    
    fprintf('darray = [ ')
    for ii=1:size(rx,1)
        fprintf('%12.4f ',rxdist(ii) )
    end
    fprintf('] \n')
    
    fprintf('flux * r^2: ')
    for ii=1:size(rx,1)
        fprintf('%12.4f ',rxdist(ii)*rxdist(ii)*powsum2(ii) )
    end
    fprintf(' \n')
    
end
fprintf('hitcount ')
disp(hitcount)


%~ figure(4)
%~ clf ;
%~ hold on;
%~ 
%~ if min(powsum1)*min(powsum2)>0
    %~ plot(log10(rxdist),log10(powsum1),'b*')
    %~ plot(log10(rxdist),log10(powsum2),'g*')
%~ 
    %~ ftlog = fittype('poly1');
    %~ [curve, gof] = fit(log10(rxdist)',log10(powsum2)',ftlog)
    %~ plot(curve,'r-')
%~ else
    %~ disp('Not all receivers received power, skip log-fit!')
%~ end

%~ figure(5)
%~ clf ;
%~ hold on;
%~ plot(rxdist,powsum1,'b*')
%~ plot(rxdist,powsum2,'g*')
%~ 
%~ bstart = -2;
%~ astart = powsum2(1)*rxdist(1)^(-bstart);
%~ 
%~ ft = fittype('a*x^b');
%~ [curve, gof] = fit(rxdist',powsum2',ft,'StartPoint',[astart,bstart])
%~ plot(curve,'r-')


% Draw n*cos(alpha) to check on the numerical precsion(should be constant)
% At the moment this is just done for the last ray that is traced
figure(3);
plot(tarray,narray.*cos(alpharray),'b')

% Draw n vs. depth
test = [];
nval = [0 0 0];
z=[zmax:-0.1:zmin];
for z1 = z
    test(end+1)=f(z1);
    nval = wshift('1D',nval,1);
    nval(3) = f(z1); % test(end);
    if (z1<= z(3) && nval(3)>nval(2) && nval(1)>nval(2) )
        disp('Local minimum of n');
        disp(z1+0.1)
        disp(nval(2))
        figure(1)
        hl = line([0,150],[z1+0.1,z1+0.1]);
        set(hl,'LineStyle','--','Color','k');
        figure(2)
        hl = line([z1+0.1,z1+0.1],[1.3,1.5]);
        set(hl,'LineStyle','--','Color','k');
    end
    
    if (z1<= z(3) && nval(3)<nval(2) && nval(1)<nval(2) )
        disp('Local maximum of n:');
        disp(z1+0.1)
        disp(nval(2))
        figure(1)
        hl = line([0,150],[z1+0.1,z1+0.1]);
        set(hl,'LineStyle',':','Color','k');
        figure(2)
        hl = line([z1+0.1,z1+0.1],[1.3,1.5]);
        set(hl,'LineStyle',':','Color','k');
        
    end
    
end

% figure(4);
% clf(4)
% hold on;
% plot(z,test,'r:')

figure(1)
%hfill1 = fill([0 100 100 0],[-3.8 -3.8 -4.8 -4.8],'y');
%set(hfill1,'LineStyle','none','FaceAlpha',0.3);
if ~isempty(rx)
    for ii=1:size(rx,1)
        h3 = line([rx(ii,1), rx(ii,1)],[rx(ii,2), rx(ii,3)]);
        set(h3,'Color','r','LineStyle','-','LineWidth',3);
    end
end

end % of function

function y = nfirn(z)

% z is negative !!!

y = 1.0 + 0.86*(1.0 - 0.638 * exp(z/34.7) );
y=1;
% add sin-behaviour
zlambda = 10;
A=0.1;
%y = y + A*cos(2*pi*z/zlambda);

sigma = 1;
zmean = -3;
A = -0.05 * (2*sigma*sigma*pi);

y = y + A/(2*sigma*sigma*pi)*exp(-(z-zmean).*(z-zmean)/(2*sigma*sigma));

%y = 1; % for tests without refraction

end % of function


function y = nfirnmeas1(z)

vdepth= [0 0.5 1 1.5 2 3 3.8 4.2 4.8 5.6 6.8 7.7 8 8.8 9.7 10.3 10.7 11.8 12.4 12.7 13.8 14.7 15.8 16.4 16.8 17.9 18.4 18.7 19.9 20.5 ];
% first value vdepth[1]=0 added by hand 3-April-2017
vdepth = vdepth*(-1);


vrho = [ 329.9 329.9 329.7 349.2 408.8 382.2 406.7 463.3 406.4 439.2 447.0 ...
    445.9 443.8 483.9 472.3 506.4 486.8 493.1 523.8 518.4 517.7 525.8 553.0 545.0 549.8 568.7 584.3 537.0 563.0 568.5];


if z<=0,
    % get rho at z from interpolation
    % use <= in next line because z is negative in ice !!
    itemp =  find (z <= vdepth);
    idepth = itemp(end);
    
    rho = vrho(idepth) + (vrho(idepth+1) - vrho(idepth))/(vdepth(idepth+1) - vdepth(idepth))*(z - vdepth(idepth));
    rho=rho/1000; %scale to density of water
    
    %disp(z)
    %disp(vdepth(idepth))
    %disp(rho)
    
    n = 1 + 0.86*rho;
else
    n = 1;
end

%plot(vdepth,vrho,'b-');

y = n;

end



