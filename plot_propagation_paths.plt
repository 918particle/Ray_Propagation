set grid
set yrange [-1200.0:100.0]
set xrange [0.0:2000.0]
unset key
set tics font "Courier,26"
set xlabel "Horizontal Distance (m)" offset 0,-2 font "Courier,26"
set ylabel "Vertical Distance (m)" offset -4,0 font "Courier,26"
set title "Index: Fit to SPICE core.  Angles: [35,45] in 0.5 deg steps." font "Courier,18"
set lmargin 14.0
set rmargin 9.0
set bmargin 7.0
set tmargin 3.0
set terminal postscript color enhanced
set output "May11_plot1.eps"

plot "data/propagation_path_35.dat" w l lw 2 lc rgb "#000000", \
"data/propagation_path_35.5.dat" w l lw 2 lc rgb "#111111", \
"data/propagation_path_36.dat" w l lw 2 lc rgb "#222222", \
"data/propagation_path_36.5.dat" w l lw 2 lc rgb "#333333", \
"data/propagation_path_37.dat" w l lw 2 lc rgb "#444444", \
"data/propagation_path_37.5.dat" w l lw 2 lc rgb "#555555", \
"data/propagation_path_38.dat" w l lw 2 lc rgb "#666666", \
"data/propagation_path_38.5.dat" w l lw 2 lc rgb "#777777", \
"data/propagation_path_39.dat" w l lw 2 lc rgb "#888888", \
"data/propagation_path_39.5.dat" w l lw 2 lc rgb "#999999", \
"data/propagation_path_40.dat" w l lw 2 lc rgb "#AAAAAA", \
"data/propagation_path_40.5.dat" w l lw 2 lc rgb "#BBBBBB", \
"data/propagation_path_41.dat" w l lw 2 lc rgb "#CCCCCC", \
"data/propagation_path_41.5.dat" w l lw 2 lc rgb "#BBCCCC", \
"data/propagation_path_42.dat" w l lw 2 lc rgb "#AACCCC", \
"data/propagation_path_42.5.dat" w l lw 2 lc rgb "#99CCCC", \
"data/propagation_path_43.dat" w l lw 2 lc rgb "#88CCCC", \
"data/propagation_path_43.5.dat" w l lw 2 lc rgb "#77CCCC", \
"data/propagation_path_44.dat" w l lw 2 lc rgb "#66CCCC", \
"data/propagation_path_44.5.dat" w l lw 2 lc rgb "#55CCCC", \
"data/propagation_path_45.dat" w l lw 2 lc rgb "#44CCCC"
