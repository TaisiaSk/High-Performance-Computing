#!/bin/bash
#SBATCH -p gnu
#SBATCH -N 1
#SPATCH -n 8
#SBATCH --ntasks-per-node=8
mpirun xhpl 
