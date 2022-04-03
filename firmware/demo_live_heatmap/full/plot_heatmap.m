clear all

% Heatmap creation
global h;
global num_nodes;
global square_dim;

num_nodes = 36;
square_dim = 6;

measurements = zeros(square_dim);
min = 100;
max = 1000;
lims = [min, max];
% h = heatmap(measurements, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'CellLabelColor', 'none', 'GridVisible', 'off');
h = heatmap(measurements, 'ColorScaling', 'scaled', 'ColorLimits', lims, 'GridVisible', 'off');
h.Title = 'Pressure Visualization Heatmap';
h.XLabel = 'col #'
h.YLabel = 'row #';
colorbar;

arduino_obj = serialport("COM9", 115200)

configureTerminator(arduino_obj, "CR/LF");
flush(arduino_obj);
arduino_obj.UserData = struct("Data", measurements, "Count", 1);
configureCallback(arduino_obj, "terminator", @plotMat);

