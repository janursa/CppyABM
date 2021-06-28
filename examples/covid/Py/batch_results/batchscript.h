#!/bin/bash
#SBATCH --partition=all
#SBATCH --time=1:00:00                           # Maximum time requested
#SBATCH --nodes=1                                 # Number of nodes
#SBATCH --chdir   ./      # directory must already exist!
#SBATCH --job-name  test
#SBATCH --output    test-%N-%j.out            # File to which STDOUT will be written
#SBATCH --error     test-%N-%j.err            # File to which STDERR will be written
#SBATCH --mail-type ALL                           # Type of email notification- BEGIN,END,FAIL,ALL
#SBATCH --mail-user jalil.nourisa@hereon.de            # Email to which notifications will be sent. It defaults to 
unset LD_PRELOAD
source /etc/profile.d/modules.sh
module load maxwell gcc/8.2
module load mpi/openmpi-x86_64
export PYTHONPATH=/usr/lib64/python3.6/site-packages/openmpi
mpiexec -np 2 --mca pml ob1 python3 ../batch_runner.py ../scenarios/cu-50-nisol-bars-only.json
