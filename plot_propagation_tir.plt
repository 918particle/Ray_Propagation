set grid
set yrange [-200.0:200.0]
set xrange [1900.0:2100.0]
unset key
set tics font "Courier,22"
set xlabel "Horizontal Distance (m)" offset 0,-2 font "Courier,26"
set ylabel "Vertical Distance (m)" offset -4,0 font "Courier,26"
set lmargin 14.0
set rmargin 9.0
set bmargin 7.0
set tmargin 3.0
set terminal postscript color enhanced
set output "June26_plot1.eps"

plot for [i=ARG1:ARG2] 'data/sp_propagation_path_'.i.'.dat' w l lw 0.2 lc rgb "#999999"
