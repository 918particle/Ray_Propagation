rm RunPropagatorFresnel
make RunPropagatorFresnel
rm data/*propagation_path*.dat
./RunPropagatorFresnel
gnuplot -c plot_propagation_fresnel.plt $1 $2 $3
evince June26_plot1.eps
