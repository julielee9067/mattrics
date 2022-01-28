clear all

% Heatmap creation
global f;
f = fopen("Jan27_6by6.csv", "wt");
meas = zeros(1, 36);

arduino_obj = serialport("COM3", 9600)

configureTerminator(arduino_obj, "CR/LF");
flush(arduino_obj);
arduino_obj.UserData = struct("Data", meas, "Count", 1);
configureCallback(arduino_obj, "terminator", @write_one_reading);

