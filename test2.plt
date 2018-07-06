set xrange [0:90]
set yrange [0:1]
unset key
set grid

plot "a.dat" using (90.0-$1):($2/100):($3/100) w yerrorbars pt 6
