set grid
set yrange [-1000.0:100.0]
set xrange [0.0:2000.0]
unset key
set tics font "Courier,26"
set xlabel "Horizontal Distance (m)" offset 0,-2 font "Courier,26"
set ylabel "Vertical Distance (m)" offset -3,0 font "Courier,26"
set lmargin 12.0
set rmargin 12.0
set bmargin 7.0
set tmargin 3.0
set terminal postscript color enhanced
set output "May11_plot1.eps"

plot "propagation_path_35.dat" w l lw 2 lc rgb "#000000", \
"propagation_path_35.5.dat" w l lw 2 lc rgb "#111111", \
"propagation_path_36.dat" w l lw 2 lc rgb "#333333", \
"propagation_path_36.5.dat" w l lw 2 lc rgb "#555555", \
"propagation_path_37.dat" w l lw 2 lc rgb "#777777", \
"propagation_path_37.5.dat" w l lw 2 lc rgb "#999999", \
"propagation_path_38.dat" w l lw 2 lc rgb "#BBBBBB", \
"propagation_path_38.5.dat" w l lw 2 lc rgb "#DDDDDD", \
"propagation_path_39.dat" w l lw 2 lc rgb "#FFFFFF", \
"propagation_path_39.5.dat" w l lw 2 lc rgb "#AA0000", \
"propagation_path_40.dat" w l lw 2 lc rgb "#AA0000"
