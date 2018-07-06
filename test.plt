set xrange [0:90]
set yrange [0:1]
unset key
set grid

plot "results_noCanReflect_surface_15cm.dat" using (90.0-$1):($2/400):($3/400) w yerrorbars pt 6, \
"results_noCanReflect_surface_15cm.dat" using (90.0-$1):($4/400):($5/400) w yerrorbars pt 6, \
"results_wCanReflect_surface_15cm.dat" using (90.0-$1):($2/400):($3/400) w yerrorbars pt 6, \
"results_wCanReflect_surface_15cm.dat" using (90.0-$1):($4/400):($5/400) w yerrorbars pt 6,
