close all;
clear;
clc;
addpath(genpath('AHRS'));
load('calib')

scale(4:6) = scale(4:6)*0.25;

% make serial device
s = InitSerial( '/dev/ttyACM3', 's' );

% make AHRS objects
AHRS = MadgwickAHRS('SamplePeriod', 1/256, 'Beta', 0.1);
%AHRS = MahonyAHRS('SamplePeriod', 1/256, 'Kp', 0.5);

% open figure
fig1 = figure(1);
pause(1/100);

step = 0;
pos = zeros(1,3);
% make acc to pos system:
accToVel = tf(1,[1,0]); % pure integrate
HipassVel = 1-tf(1,[2,1]); % Hipass Vel to remove drift
VelToPos = tf(1,[1,0]); % pure integrate
accToPos = accToVel*HipassVel*HipassVel*VelToPos; % continous system.
accToPos = c2d(accToPos,1/1000); % make discrete model
[num,den] = tfdata(accToPos);
accToPos = dfilt.df2(num{1},den{1}); % convert to filter
accToPos.PersistentMemory = true;

while ~keypressInFig( fig1, 'x' )
    dataUncalib = getFrames( s, zeros(0,6) );
    data = ApplyCalib( dataUncalib, offset, scale );
    
    size_ = size(data);
    size_ = size_(1);
    step = step + size_;
    step = step(1);
   
    y = zeros(size_,3);
    for t = 1:size_
        AHRS.UpdateIMU(data(t,4:6) .* (pi/180), data(t,1:3)./9.82);	% gyroscope units must be radians, accelerometer must be in g
        %AHRS.Update( data(t,4:6) .* (pi/180), data(t,1:3)./9.82, [0, 1, 0] );	% gyroscope units must be radians, accelerometer must be in g, fake magnetometer
        q = AHRS.Quaternion;
        %get and rotate acceleration
        acc = [ 0, data(t,1:3) ];
        acc = quaternProd( q, quaternProd( acc, quaternConj(q) ) );
        acc = acc(2:4);
        acc = acc - [0, 0, 9.82];
        y(t,:) = acc;
        %pos = filter( accToPos, acc );
    end
    pos = filter( accToPos, y, 1 );
    
    if step > 33
        acc_is = data(end,1:3)
        pos_is = pos(end,:)
        step = step - 33;
        euler = quatern2euler(quaternConj(q)) * (180/pi);
        [ax, ang] = quatern2axisAngle(q);
        [X,Y,Z] = peaks(25);
        Z = Z.*0.003;
        X = X.*0.03;
        Y = Y.*0.03;
        %Calculate inverse transformed rotation:
        posT = [ 0, pos(end,:) ];
        posT = quaternProd( quaternConj(q), quaternProd( posT, q ) );
        posT = posT(2:4);
        X = X + posT(1);
        Y = Y + posT(2);
        Z = Z + posT(3);
        hold off
        h = surf(X,Y,Z);
        hold on
        %plot3( [pos(1), pos(1)+acc(1)], [pos(2), pos(2)+acc(2)], [pos(3), pos(3)+acc(3)] );
        rotate( h, ax, ang*(180/pi), [0,0,0] );
        axis equal;
        pause(1/10000);
        % rotate
    end
end


EndSerial(s)

