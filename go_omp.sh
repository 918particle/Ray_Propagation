make clean
make RunPropagatorOMP
rm data/*propagation_path*.dat
./RunPropagator
gnuplot -c plot_propagation_loop.plt $1 $2 $3
evince June13_plot1.eps
