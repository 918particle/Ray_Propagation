set grid
set key box on font "Courier,22"
z_0_shelf = 37.0
z_0_sheet = 61.0
n_i = 1.78
n_s_shelf = 1.3
n_s_sheet = 1.33
x0=0
z0=0
f(x) = -1.0/(2.0*z_0_shelf)*(n_i-n_s_shelf)/(n_s_shelf)*(x-x0)**2+z0
g(x) = -1.0/(2.0*z_0_sheet)*(n_i-n_s_sheet)/(n_s_sheet)*(x-x0)**2+z0
set xrange [-50:50]
set yrange [-25:25]
set xlabel "y (m)" font "Courier,24" offset 0,-2
set ylabel "z (m)" font "Courier,24" offset -2,0
set ytics font "Courier,24"
set xtics font "Courier,24"
set terminal postscript color enhanced
set output "Jan24_plot1.eps"
plot f(x) w l lw 3 lc rgb "#000000" title "Shelf", g(x) w l lw 3 lc rgb "#999999" title "Sheet"
