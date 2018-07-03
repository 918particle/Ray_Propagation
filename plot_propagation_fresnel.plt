set grid
set yrange [-820:-780]
set xrange [1990.0:2010.0]
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

plot for [i=ARG1:ARG2] for [j=0:ARG3] 'data/sp_propagation_'.i.'_'.j.'.dat' w l lw 0.2 lc rgb "#999999"
