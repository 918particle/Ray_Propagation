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

plot "propagation_path_35.dat" w l lw 2 lc rgb "#000000", \
"propagation_path_35.5.dat" w l lw 2 lc rgb "#111111", \
"propagation_path_36.dat" w l lw 2 lc rgb "#222222", \
"propagation_path_36.5.dat" w l lw 2 lc rgb "#333333", \
"propagation_path_37.dat" w l lw 2 lc rgb "#444444", \
"propagation_path_37.5.dat" w l lw 2 lc rgb "#555555", \
"propagation_path_38.dat" w l lw 2 lc rgb "#666666", \
"propagation_path_38.5.dat" w l lw 2 lc rgb "#777777", \
"propagation_path_39.dat" w l lw 2 lc rgb "#888888", \
"propagation_path_39.5.dat" w l lw 2 lc rgb "#999999", \
"propagation_path_40.dat" w l lw 2 lc rgb "#AAAAAA", \
"propagation_path_40.5.dat" w l lw 2 lc rgb "#BBBBBB", \
"propagation_path_41.dat" w l lw 2 lc rgb "#CCCCCC", \
"propagation_path_41.5.dat" w l lw 2 lc rgb "#DDDDDD", \
"propagation_path_42.dat" w l lw 2 lc rgb "#DDEEEE", \
"propagation_path_42.5.dat" w l lw 2 lc rgb "#CCEEEE", \
"propagation_path_43.dat" w l lw 2 lc rgb "#BBEEEE", \
"propagation_path_43.5.dat" w l lw 2 lc rgb "#AAEEEE", \
"propagation_path_44.dat" w l lw 2 lc rgb "#99EEEE", \
"propagation_path_44.5.dat" w l lw 2 lc rgb "#88EEEE", \
"propagation_path_45.dat" w l lw 2 lc rgb "#77EEEE"
