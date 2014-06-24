%Rygaard IR

analysis = load( 'analysis1.csv' );
%time, pos, motor, pos estimate, speed est, acc est, ir dist, acc raw x,
%acc raw y, acc raw z

analysis = analysis(5:end,:);

Fs_c = 200;
Fs_e = 1000;

ana_time = linspace( 0, length(analysis)/Fs_c, length(analysis) );




%plot stuff
close all
hold on

% plot setpoint
plot( ana_time, analysis(:,end), '-','Color', [0,0.5,0],'LineWidth',0.1 );
% plot 1 cm border
plot( ana_time,[analysis(:,end)+0.01,analysis(:,end)-0.01], '-','Color', [0.5,0.5,0.5],'LineWidth',0.1 );
% plot real angle
window = 10;
plot( ana_time, filtfilt(ones(1,window),window,sin(analysis(:,2)))-0.0175, '-','Color', [0,0,1],'LineWidth',0.1 );
% plot real pos
window = 10;
plot( ana_time, filtfilt(ones(1,window),window,sin(analysis(:,3))), '-','Color', [1,0,0],'LineWidth',0.1 );

xlabel( 'time [s]' )
ylabel( 'distance [m] / angle [rad]' )
legend(...
    'set point',...
    '+ 1cm','- 1cm',...
    'Measured angle [rad]',...
    'Measured altitude [m]',...
    'Location', 'NorthOutside'...
);

cleanfigure('minimumPointsDistance',0.05);
saveas( gcf, 'fig', 'pdf' )
matlab2tikz('plot.tikz', 'height', '\figureheight', 'width', '\figurewidth', 'extraAxisOptions', 'legend columns = 2');
