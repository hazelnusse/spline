set pagination off

define print_point_sequence
    printf "["
    set $i = $arg0
    while ($i < $arg1 - 1)
        printf "(%f, %f), ", (*$i)[0], (*$i)[1]
        set $i = $i + 1
    end
    printf "(%f, %f)]\n", (*$i)[0], (*$i)[1]
end

break spline::de_casteljau<>:on_entry
    commands
    silent
    set $i = first
    printf "Input:\n"
    print_point_sequence first last
    set $d_first_orig = d_first
    set $d_last_orig = d_first + 2 * (num_coefficients - 1) + 1
    printf "Initial output:\n"
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

break de_casteljau.hpp:41
    commands
    silent
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

break de_casteljau.hpp:48
    commands
    silent
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

break de_casteljau.hpp:78
    commands
    silent
    printf "After row 0 to row 1, output:\n"
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

break de_casteljau.hpp:98
    commands
    silent
    printf "After row %d to row %d, output:\n", r - 1, r
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

break de_casteljau.hpp:103
    commands
    silent
    printf "After row %d to row %d, output:\n", num_coefficients - 2, num_coefficients - 1
    print_point_sequence $d_first_orig $d_last_orig
    continue
end

run
quit
