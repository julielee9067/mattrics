clear all

% Heatmap creation
global f;
f = fopen("overnight_data_results.csv", "at");
meas = zeros(1, 36);

arduino_obj = serialport("COM4", 9600)

configureTerminator(arduino_obj, "CR/LF");
flush(arduino_obj);
arduino_obj.UserData = struct("Data", meas, "Count", 1);
configureCallback(arduino_obj, "terminator", @write_one_reading);

