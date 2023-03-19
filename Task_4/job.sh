#!/bin/bash
#SBATCH -p gnu
#SBATCH -c 4
export OMP_NUM_TREADS=4
./a.out
