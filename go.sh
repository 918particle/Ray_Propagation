rm Run
make Run
rm data/*propagation*.dat
./Run
gnuplot -c plot_propagation_fresnel.plt $1 $2 $3
evince June26_plot1.eps
