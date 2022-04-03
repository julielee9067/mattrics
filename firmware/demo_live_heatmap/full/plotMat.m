function plotMat(src, ~)
    global h;
    global square_dim;
    global num_nodes;

    data_string = readline(src)
    src.UserData.Count = src.UserData.Count + 1;

    if src.UserData.Count > 3
        configureCallback(src, "off");
        data_string_split = split(data_string);
        data_string_split(end) = [];
        data_string_split = transpose(data_string_split);
        data_double = str2double(data_string_split)
        data2d = reshape(data_double, [], square_dim)

        h.ColorData = data2d;
        drawnow;
        configureCallback(src, "terminator", @plotMat);
    end
end

