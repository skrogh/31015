%% conts
port = 'COM10';
history = 5000;


%% Save old data i case of crash
if exist('dataUncalib','var') ~= 0
    save( ['dataFile' dec2hex(fileNr,3) 'crash.txt'], 'dataUncalib', '-ascii' );
end

%% Clear char buffer for figures
try
    close( fig1 );
end
try
    close( fig2 );
end

%% Open serial device
dataPlot = [0, 0, 0, 0, 0, 0];
dataUncalib = [];
fig1 = figure(1);
clf(1)

% Generate colors
clr = lines(6);

% Generate figures and plots
for i=1:6
    if i==4
        fig2 = figure(2);
        clf(2)
    end
    hold on
    plots(i) = plot(dataPlot(:,i),'Color',clr(i,:));
    axis 'auto'
    %ylim([0 2.^15]);
    hold off
    set(plots(i),'YDataSource', ['dataPlot(:,' num2str(i) ')']);
end

% Give plots time to open
pause(1/5);

% Check if serial port is open, if then close it
if exist('s','var') ~= 0
    fclose(s);
    delete(s);
    clear s;
end

% Open serial port
if exist('s','var') == 0
    s = serial(port,'BaudRate', 500000, 'InputBufferSize', 2048);
    fopen(s);
end

display( 'Data capture starting, end it by pressing "x" in one of the display windows' );

test = 0;
err = '';
%try
while ( ~keypressInFig( fig1, 'x' ) && ~keypressInFig( fig2, 'x' ) )
    dataUncalib = getFrames( s, dataUncalib );
    
    if mod(test,5)== 0
        if length(dataUncalib) > 1000
            if exist('offset','var')
                dataPlot = power_plot( dataUncalib, history, offset, scale );
            else
                dataPlot = power_plot( dataUncalib, history );
            end
        end
        for i=1:6
            refreshdata(plots(i))
        end
        pause(1/1000);
        
        % Print buffer size
        s.BytesAvailable
    end
    
    test = test+1;
end
%catch err
%    rethrow(err)
%    display( 'OOps something bad happend!' );
%end


%% Close connection
fclose(s);
delete(s);
clear s;

%% Clear char buffer for figures
try
    close( fig1 );
end
try
    close( fig2 );
end

fileNr = fileNr + 1;
save( ['dataFile' dec2hex(fileNr,3) '.txt'], 'dataUncalib', '-ascii' );

%