function [ offset, scale ] = InteractiveCalib( ) %Opens connection, calibrates and closes
s = InitSerial( 'COM10', 's' );

dataUncalib=zeros(0,6);
display('Please lay device flat, and keep it steady')
while (length(dataUncalib)<5000) %sample for 10s
    dataUncalib = getFrames( s, dataUncalib );
end
display('Gyro calib done');
[ offsetG, scaleG ] = Calib( dataUncalib, 1 );
pause;

dataUncalib=zeros(0,6);
display('Rotate, then hold steady, repeat')
while (length(dataUncalib)<20000) %sample for 10s
    dataUncalib = getFrames( s, dataUncalib );
end
display('Accelerometer calib done');
[ offsetA, scaleA ] = Calib( dataUncalib, 0 );

offset = [offsetA, offsetG];
scale = [scaleA, scaleG];

EndSerial(s)

end

