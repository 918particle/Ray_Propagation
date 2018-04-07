make clean
make RunPropagator
rm data/propagation_path*.dat
./RunPropagator
gnuplot plot_propagation_loop.plt
