make clean
make RunPropagatorTIR
rm data/*propagation_path*.dat
./RunPropagatorTIR
gnuplot -c plot_propagation_tir.plt $1 $2
evince June26_plot1.eps
