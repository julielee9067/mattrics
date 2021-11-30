function matrix_2d = convert_1d_to_2d_matrix(matrix_1d, r, c)
matrix_2d = zeros(r, c);
    for row = 1:r
        for col = 1:c
            matrix_2d(row, col) = matrix_1d((row - 1) * r + col); 
        end
    end           
end

