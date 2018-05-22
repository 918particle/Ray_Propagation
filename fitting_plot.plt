set grid
set key top center box on font "Courier,16" spacing 3.5 width 2.0
set bmargin 5
set lmargin 10
set rmargin 10
set tmargin 3
set datafile separator ","
set pointsize 0.5
g(x) = 1.0/n / cos(x)
d = -1
e = 1.0
fit [0:2] g(x) 'linear.dat' via n
set yrange [-10:10]
set xrange [-2:2]
set xlabel "Initial angle (radians)" font "Courier,20"
set ylabel "C_0" font "Courier,20"
set xtics font "Courier,20"
set ytics font "Courier,20"
set terminal postscript enhanced color
set output "May21_plot1.eps"

plot 'linear.dat' w p pt 6 lc -1 title "Evaluating Eq. 19", g(x) lc 7 lw 2.5 title "g(x) = n^{-1} sec(x)", -g(x) lc 3 lw 2.5 title "-g(x), w/ n = 1.7009 +/- 0.0003"