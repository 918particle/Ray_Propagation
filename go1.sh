g++ RunPropagator.cc -o RunPropagator Propagator.cc Reflector.cc Emitter.cc Ice.cc
./RunPropagator
gnuplot plot_propagation_loop.plt
rm propagation_path*.dat
