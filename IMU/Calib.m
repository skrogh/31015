function [offset, scale] = Calib( dataUncalib, flag ) % flag = 0, calib acc, flag = 1 calib gyro
if flag == 0
    %Physical constants:
    g = 9.82;
    
    %lowpas filter
    windowSize = 500;
    dataFilt = filter(ones(1,windowSize)/windowSize,1,dataUncalib);
    
    % Set stability threshold:
    thresh = 1;
    
    % Plot filtered data
    figure(1)
    plot(dataFilt);
    
    % Only use stablie points, finde N for speed
    stabPoints = 0;
    for i=2:length(dataUncalib)
        if abs( dataFilt(i,:)-dataFilt(i-1,:) ) < thresh
            stabPoints = stabPoints+1;
        end
    end
    
    % Only use stabile points, find them
    dataStab = zeros( stabPoints, 6 );
    j=1;
    for i=2:length(dataUncalib)
        if abs( dataFilt(i,:)-dataFilt(i-1,:) ) < thresh
            dataStab(j,:) = dataFilt(i,:);
            j = j+1;
        end
    end
    
    % Plot stabile points, 2D
    figure(2)
    plot(dataStab);
    
    % --||-- 3D
    figure(3)
    scatter3( dataStab(1:50:end,1),dataStab(1:50:end,2),dataStab(1:50:end,3) );
    axis equal
    
    % Find best ellipsoide
    [ center, radii, evecs, v ] = ellipsoid_fit( dataStab(:,1:3), 1 );
    
    % try plotting best fit (fails if no data)
    hold on
    try
        ellipsoid( center(1),center(2),center(3), radii(1), radii(2), radii(3) );
    end
    hold off
    
    offset = transpose(center);
    scale = transpose(g./radii);
    
    % Plot calibrated data
    figure(4)
    
    ApplyCalib( dataStab(:,1:3), offset, scale );
    scatter3( dataStab(1:50:end,1),dataStab(1:50:end,2),dataStab(1:50:end,3) );
    axis equal
else
    %Gyro datasheet:
    deg_s = 1000;
    full_deg_s = deg_s/2;
    full_bits = 2^16;
    scaleG = full_deg_s/full_bits;
    

    scale = [ scaleG, scaleG, scaleG ];
    offset = mean( dataUncalib(:,4:6) );
    figure(1)
    data = ApplyCalib( dataUncalib(:,4:6), offset, scale );
    plot(data);
end