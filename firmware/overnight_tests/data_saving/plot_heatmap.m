clear all

% Heatmap creation
global h;
meas = zeros(11);
min = 150;
max = 550;
lims = [min, max];
%h = heatmap(meas, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'CellLabelColor', 'none', 'GridVisible', 'off');
h = heatmap(meas, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'GridVisible', 'off');
h.Title = 'Force Application Heatmap';
h.XLabel = 'col #';
h.YLabel = 'row #';
colorbar;

arduino_obj = serialport("COM3", 9600)

configureTerminator(arduino_obj, "CR/LF");
flush(arduino_obj);
arduino_obj.UserData = struct("Data", meas, "Count", 1);
configureCallback(arduino_obj, "terminator", @plotMat);

