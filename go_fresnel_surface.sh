rm RunPropagatorFresnelSurface
make RunPropagatorFresnelSurfaceOMP
rm data/*propagation_path*.dat
./RunPropagatorFresnelSurface
gnuplot -c plot_propagation_fresnel_surface.plt $1 $2 $3
evince June26_plot1.eps
