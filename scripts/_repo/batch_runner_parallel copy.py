from batch_runner import BatchRunner
from mpi4py import MPI

class ParallelBatchRunner(BatchRunner)

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
        

        def run_model(start,end):
            """
            Constructs and runs the model 
        
            """
            import time

            outputs = []
            CPU_time = []
            for i in range(start,end):
                begin = time.time()
                model = self.model_class(**self.args)
                output = model.episode()
                end = time.time()
                outputs.append(output)
                CPU_time.append(end-begin)

            return outputs,CPU_time
        outputs_perCore,CPU_time_perCore = run_model(portion[0],portion[1])
        outputs_stacks = self.comm.gather(outputs_perCore,root = 0)
        CPU_time_stacks = self.comm.gather(CPU_time_perCore,root = 0)
        if self.rank == 0:
            import numpy as np
            outputs = np.array([])
            CPU_time = np.array([])
            for output,time in zip(outputs_stacks,CPU_time_stacks):
                outputs = np.concatenate([outputs,output],axis = 0)
                CPU_time = np.concatenate([CPU_time,time],axis = 0)
            keys = outputs[0].keys()
            cumulated_data = {}
            for key in keys:
                aa = []
                for output in outputs:
                    aa += output[key]
                cumulated_data.update({key:aa}) 

            return cumulated_data,CPU_time
