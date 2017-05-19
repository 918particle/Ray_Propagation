make clean
make
rm propagation_path*.dat
./RunPropagator
gnuplot plot_propagation_loop.plt
rm propagation_path*.dat
