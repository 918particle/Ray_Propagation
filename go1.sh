make clean
make RunPropagatorOMP
rm data/propagation_path*.dat
./RunPropagator
gnuplot plot_propagation_loop.plt
evince June6_plot3.eps
