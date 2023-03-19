#!/bin/bash
#SBATCH -p gnu
#SBATCH -c 12
export OMP_NUM_TREADS=12
./a.out
