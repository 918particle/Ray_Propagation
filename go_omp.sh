make clean
make RunPropagatorOMP
rm data/propagation_path*.dat
./RunPropagator
gnuplot -e "max='$1'" plot_propagation_loop.plt
evince June6_plot3.eps
