make clean
make RunPropagatorTIR
rm data/*propagation_path*.dat
./RunPropagatorTIR
gnuplot -c plot_propagation_loop.plt $1 $2 $3
evince June26_plot1.eps
