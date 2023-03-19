#!/bin/bash
#SBATCH -p gnu
#SBATCH -c 8
export OMP_NUM_TREADS=8
./a.out
