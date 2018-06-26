make clean
make RunPropagatorPerturbationsOMP
rm data/*propagation_path*.dat
./RunPropagatorPerturbations
gnuplot -c plot_propagation_loop.plt $1 $2 $3
evince June25_plot$4.eps
