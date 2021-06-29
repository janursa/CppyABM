import time
import os

class Runner:

    def __init__(self,model_class,iter_n, args={},parallel=False):
        self.model_class = model_class
        self.args = args
        self.iter_n = iter_n
        if parallel:
            from mpi4py import MPI
            self.comm = MPI.COMM_WORLD
            self.rank = self.comm.Get_rank()
        else:
            self.rank = 0
        if self.rank == 0:
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
        

        def run_model(start,end):
            """
            Constructs and runs the model 
        
            """
            outputs = []
            for i in range(start,end):
                model = self.model_class(**self.args)
                output = model.episode()
                outputs.append(output)

            return outputs
        outputs_perCore = run_model(portion[0],portion[1])
        outputs_stacks = self.comm.gather(outputs_perCore,root = 0)
        if self.rank == 0:
            import numpy as np
            outputs = np.array([])
            for stack in outputs_stacks:
                outputs = np.concatenate([outputs,stack],axis = 0)
            keys = outputs[0].keys()
            cumulated_data = {}
            for key in keys:
                aa = []
                for output in outputs:
                    aa += output[key]
                cumulated_data.update({key:aa}) 
            return cumulated_data
