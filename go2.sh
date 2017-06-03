g++ MooresBayModel.cc -o MooresBayModel Propagator.cc Reflector.cc Emitter.cc Ice.cc
./MooresBayModel
gnuplot plot_propagation_loop.plt
rm propagation_path*.dat
