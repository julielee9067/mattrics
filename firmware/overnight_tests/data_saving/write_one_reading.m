function write_one_reading(src, ~)
    global f;
    data_string = readline(src)
    src.UserData.Count = src.UserData.Count + 1;

    if src.UserData.Count > 2
        configureCallback(src, "off");
        [h,m,s] = hms(datetime);
        fprintf(f, "%.0f:%.0f:%.0f\n", h, m, s);
        fprintf(f, "%s\n", data_string);
        configureCallback(src, "terminator", @write_one_reading);
    end
end

