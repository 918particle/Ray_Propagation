#############################################################
#############################################################
#############################################################
##############                                  #############
##############   Ray Propagation - ARIANNA UCI  #############
##############                                  #############
#############################################################
#############################################################
#############################################################

Before using the simulation, you need to setup an environment variable.

1) open up .bash_profile or .bashrc in home directory.

2) at the end of the file type: 
	export RAYPROP="<PATH TO RAY_PROPAGATION>/Ray_Propagation"

3) save and close the file.

4) In the command line type:
	source .bash_profile
   or
	source .bashrc
   depend on which file you altered.

Now the simulation should work. You can compile and run like usual cpp code. Note that the simulation requires input variables which can be found in the first function under Propagator.cc. Alternatively, you can alter Hub.py to your liking. A template for altering can be found at the end of Hub.py just after all of the function definitions. Note that using Hub.py will save the simulation output in the new RAYPROP directory that you just defined.


*************** WARNING: *********************
The code will empty your trash many times. To prevent this, comment out the lines in Hub.py that have:
	os.system('rm -rf ~/.local/share/Trash/*')
This was done to save storage during heavy runs.
***********************************************

-Have Fun!