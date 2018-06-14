make clean
make RunPropagator
rm data/*propagation_path*.dat
./RunPropagator
gnuplot -e "max='$1'" plot_propagation_loop.plt
evince June13_plot1.eps
