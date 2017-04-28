#!/bin/bash
#SBATCH --job-name=mainjob1
#SBATCH --mail-type=ALL
#SBATCH --mail-user=mcglon@rpi.edu

srun -o results/n1c10.log -N 1 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
srun -o results/n2c10.log -N 2 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
srun -o results/n4c10.log -N 4 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
srun -o results/n8c10.log -N 8 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
srun -o results/n16c10.log -N 16 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
srun -o results/n32c10.log -N 32 --ntasks-per-node=1 --overcommit ~/barn/builds/models/TSP-Ross/tsps --synch=3 --cities=10 --avl-size=26 --extramem=10000000 --end=1200&
wait
