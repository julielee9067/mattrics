function plotMat(src, ~)
    global h;
    data_string = readline(src);
    src.UserData.Count = src.UserData.Count + 1;

    if src.UserData.Count > 2
        configureCallback(src, "off");
        data_string_split = split(data_string);
        data_string_split(end) = [];
        data_string_split = transpose(data_string_split);
        data_double = str2double(data_string_split)
        data2d = convert_1d_to_2d_matrix(data_double, 6, 6)
        h.ColorData = data2d;
        drawnow;
        configureCallback(src, "terminator", @plotMat);
    end
end

