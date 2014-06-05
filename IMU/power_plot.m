function [ dataOut ] = power_plot( data, N, offset, scale )

if length(data)>N
    dataOut = data( length(data)-N:end ,:);
else
    dataOut = data;
end

if exist('offset','var')==1
    dataOut = bsxfun(@minus, dataOut, offset);
    dataOut = bsxfun(@times, dataOut, scale);
end
% Hipass filter
windowSize = 50;
%dataOut = filtfilt([ 1, -ones(1,windowSize)/windowSize ],1,dataOut);

% Recify (or square)
%dataOut = dataOut.*dataOut;
%dataOut = abs( dataOut );

% Mean
windowSize = 50;
%dataOut = filter(ones(1,windowSize)/windowSize,1,dataOut);

% sqrt?
%dataOut = sqrt(dataOut);


end

