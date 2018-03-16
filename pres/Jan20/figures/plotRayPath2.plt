set grid
set key box on font "Courier,22"

z_0 = 61.0
z_1 = -10
y_1 = 10
n_i = 1.78
n_s = 1.3
Q1 = 1 + n_i/n_s
Q0 = z_1/z_0+1 + n_i/(n_i-n_s)*(log(n_s/(n_i-n_s))-2.0)
f(x) = -0.5*Q1/z_0*(x-y_1)**2+Q0/Q1*z_0

set xrange [-50:50]
set yrange [-250:250]
set xlabel "y (m)" font "Courier,24" offset 0,-1.5
set ylabel "z (m)" font "Courier,24" offset -1.5,0
set ytics font "Courier,24"
set xtics font "Courier,24"
set terminal postscript color enhanced
set output "Jan24_plot1.eps"
plot f(x) w l lw 3 lc rgb "#000000" title "Shelf"
