set grid
set datafile separator ","
set pointsize .4
f(x) = 1.0/a * tan(x) - (b*x)
fit [-2:2] f(x) 'linear.dat' via a,b
set yrange [-5:5]
set xrange [-3.14159/2.0:3.14159/2.0]

plot 'linear.dat' w p pt 6, f(x)
