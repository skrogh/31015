function [ dataPlot, thisFrame ] = getFrames( s, dataPlot )
%% read all frames and append to dataPlot
while( s.BytesAvailable==0 ) % Wait for something
end

data = uint8(0);

while bitand( data, 128 ) == uint8(0)
    data = fread( s, 1 ); % Read first
end

while( s.BytesAvailable<13 ) % Wait for the rest 13 bytes
end
dataFrames = floor((s.BytesAvailable-13)/14);
dataFull = [ data; fread( s, 13+dataFrames*14 ) ];

%% Process each frame
dataAllFrames = zeros(dataFrames+1,6);

for j=0:dataFrames
    dataFrame = dataFull(j*14+1:j*14+14);
    
    dataFrame(1) = bitand( dataFrame(1), 127 );
    % Do something with frame
    dataUnpacked = bitUnpack( dataFrame );
    
    dataArranged = zeros(1,6);
    for i=1:6
        dataArranged(i) = typecast([uint8(dataUnpacked(i*2)),...
            uint8(dataUnpacked(i*2-1))], 'int16');
        
    end
    
    dataAllFrames(j+1,:) = dataArranged;
end

thisFrame = dataPlot;
dataPlot = [dataPlot; dataAllFrames ];
    
end

