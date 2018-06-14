set grid
set yrange [-1000.0:1000.0]
set xrange [0.0:4000.0]
unset key
set tics font "Courier,22"
set xlabel "Horizontal Distance (m)" offset 0,-2 font "Courier,26"
set ylabel "Vertical Distance (m)" offset -4,0 font "Courier,26"
set lmargin 14.0
set rmargin 9.0
set bmargin 7.0
set tmargin 3.0
set terminal postscript color enhanced
set output "June13_plot1.eps"

plot for [i=0:max] 'data/sp_propagation_path_'.i.'.dat' w l lw 0.2 lc rgb "#999999", 'data/mb_propagation_path_'.i.'.dat' w l lw 0.2 lc rgb "#222299"
