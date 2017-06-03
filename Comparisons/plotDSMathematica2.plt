set grid
set xlabel "Horizontal Distance (m)" font "Courier,20" offset 0,-2
set ylabel "Vertical Distance (m)" font "Courier,20" offset -2,0
set xtics font "Courier,20"
set ytics font "Courier,20"
set key bottom center box on font "Courier,18"
set pointsize 2.5
f(x) = a*x**2+b*x+c
g(x) = d*x**2+e*x+f
fit [x=300:700] f(x) 'ds2.dat' via a,b,c
fit [x=300:700] g(x) 'ds3.dat' via d,e,f
set terminal postscript color enhanced
set output "May26_plot2.eps"

plot "ds2.dat" w p pt 6 lc -1 title "Digitization #2 from Mathematica", f(x) lw 2 lc 1 title "Fit to #1", "ds3.dat" w p pt 2 lc -1 title "Digitization #3 from Mathematica", g(x) title "Fit to #3" lw 2 lc 2
