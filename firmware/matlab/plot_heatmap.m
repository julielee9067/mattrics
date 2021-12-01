clear all

% Heatmap creation
global h;
meas = zeros(6);
min = 220;
max = 2200;
lims = [min, max];
%h = heatmap(meas, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'CellLabelColor', 'none', 'GridVisible', 'off');
h = heatmap(meas, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'GridVisible', 'off');
h.Title = 'Force Application Heatmap';
h.XLabel = 'col #';
h.YLabel = 'row #';
colorbar;

arduino_obj = serialport("COM4", 9600)

configureTerminator(arduino_obj, "CR/LF");
flush(arduino_obj);
arduino_obj.UserData = struct("Data", meas, "Count", 1);
configureCallback(arduino_obj, "terminator", @plotMat);

