set key box on font "Courier,22" width 3.75 height 1.5
z_0_shelf = 36.0
z_0_sheet = 74.0
n_i = 1.78
n_s_shelf = 1.35
n_s_sheet = 1.3
x0=0
z0=0
f(x) = -1.0/(2.0*z_0_shelf)*(n_i-n_s_shelf)/(n_s_shelf)*(x-x0)**2+z0
g(x) = -1.0/(2.0*z_0_sheet)*(n_i-n_s_sheet)/(n_s_sheet)*(x-x0)**2+z0
set lmargin 10
set rmargin 10
set bmargin 5
set tmargin 5
set xrange [-70:70]
set yrange [-35:20]
set xlabel "y (m)" font "Courier,24" offset 0,-0.5
set ylabel "z (m)" font "Courier,24" offset -1.2,0
set ytics font "Courier,24"
set xtics font "Courier,24"
set terminal postscript color enhanced
set output "Jan24_plot1.eps"
plot f(x) w l lw 5 lt 1 lc rgb "#000000" title "Shelf (z_0 = 36 meters)", g(x) w l lw 5 lt 0 lc rgb "#000000" title "Sheet (z_0 = 74 meters)"
