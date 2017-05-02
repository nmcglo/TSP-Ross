# TSP-Ross
ROSS implementation of TSP solver.

# Installation

This model requires the installation of [ROSS](http://github.com/carothersc/ROSS). After cloning the ROSS repository, follow the installation instructions in the ROSS repository to install.

Then symlink the TSP-ROSS repository into the models folder of your ROSS source. Be sure to configure your ROSS installation to 'Build Models' in the CCMAKE configurator.

You can make the model by using the command:

`make tsps'

# Running

Once you've built the program you can run it using:

`mpirun -n 4 ./tsps --cities=10 --synch=3'

This runs the program to solve a random 10 city network with 4 PEs.

You may need to increase the amount of extra events allowed by using --extramem=1000000 as well as increasing the size of the AVL tree via --avl-size=26

