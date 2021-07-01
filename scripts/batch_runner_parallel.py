from batch_runner import BatchRunner
from mpi4py import MPI

class ParallelBatchRunner(BatchRunner):

    def __init__(self,model,iter_n):
        super().__init__(model,iter_n)

        self.comm = MPI.COMM_WORLD
        self.rank = self.comm.Get_rank()

        print("Number of CPUs assigned: ",self.comm.Get_size())
    def run(self):
        if self.rank == 0:
            import numpy as np
            CPU_n = self.comm.Get_size()
            shares = np.ones(CPU_n,dtype=int)*int(int(self.iter_n)/CPU_n)
            plus = self.iter_n%CPU_n
            for i in range(plus):
                shares[i]+=1

            portions = []
            for i in range(CPU_n):
                start = sum(shares[0:i])
                end = start + shares[i]
                portions.append([start,end])
            

        else:
            portions = None

        portion = self.comm.scatter(portions,root = 0)    
        
        self.run_model(portion[0],portion[1])
        
