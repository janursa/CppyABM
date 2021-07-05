from batch_runner import BatchRunner
from mpi4py import MPI



class ParallelBatchRunner(BatchRunner):
    """
    Based on BatchRunner class, designed to execute the runs in parallel.
    """

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
        
class ParallelBatchRunner_DataCollector(BatchRunner):
    """
    Similar to ParallelBatchRunner, designed to execute the runs in parallel, gather the results from each run, stack them, and return back.
    """

    def __init__(self,model,iter_n):
        super().__init__(model,iter_n)

        self.comm = MPI.COMM_WORLD
        self.rank = self.comm.Get_rank()

        print("Number of CPUs assigned: ",self.comm.Get_size())
    def run_model(self,start,end):
        """
        Runs the model and returns the results
    
        """
        stack_results_per_core = []
        for i in range(start,end):
            self.model.reset(i)
            results = self.model.episode()
            stack_results_per_core.append(results)
            print("The iteration {} is completed.".format(i))
        return stack_runs_per_core

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
        
        outputs_perCore = self.run_model(portion[0],portion[1])

        outputs_stacks = self.comm.gather(outputs_perCore,root = 0)
        if self.rank == 0:
            import numpy as np
            outputs = np.array([])
            for output in outputs_stacks:
                outputs = np.concatenate([outputs,output],axis = 0)
            keys = outputs[0].keys()
            cumulated_data = {}
            for key in keys:
                aa = []
                for output in outputs:
                    aa += output[key]
                cumulated_data.update({key:aa}) 

            return cumulated_data