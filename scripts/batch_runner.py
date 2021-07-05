
class BatchRunner:

    def __init__(self,model,iter_n):
        self.model = model
        self.iter_n = iter_n
    def run_model(self,start,end):
        """
        Runs the model 
    
        """
        for i in range(start,end):
            self.model.reset(i)
            self.model.episode()
            print("The iteration {} is completed.".format(i))

    
