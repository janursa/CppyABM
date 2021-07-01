from batch_runner import BatchRunner

class SerialBatchRunner(BatchRunner):

    def __init__(self,model,iter_n):
        super().__init__(model,iter_n)


    def run(self):
        for i in range(self.iter_n):
            self.model.reset(i)
            self.model.episode()
