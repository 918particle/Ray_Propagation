set grid
set yrange [-500.0:500.0]
set xrange [-1000:1000.0]
unset key
set tics font "Courier,22"
set xlabel "Horizontal Distance (m)" offset 0,-2 font "Courier,26"
set ylabel "Vertical Distance (m)" offset -4,0 font "Courier,26"
set title "Index: Fit to South Pole. [-70.0,-80.0] deg in 0.5 deg steps." font "Courier,18"
set lmargin 14.0
set rmargin 9.0
set bmargin 7.0
set tmargin 3.0
#set style line 1 linecolor -1 linewidth 0.8 dashtype 2 pointtype 2 pointsize default pointinterval 1
#set arrow 1 from 0,-100 to 4000,-100 nohead ls 1
set terminal postscript color enhanced
set output "May19_plot1.eps"

plot for [i=0:2099] 'propagation_path_'.i.'.dat' w l lw 0.2 lc rgb "#999999"
