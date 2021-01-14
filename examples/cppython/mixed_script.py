import numpy as np
from random import randrange
import sys
import os
import platform
import pathlib
import pandas as pd
import time
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'build'))
from myBinds import Domain, Cell
import torch
import torch.nn.functional as F
import numpy as np
class policy(torch.nn.Module):
    """
    implements a policy model
    """
    def __init__(self):
        super(policy, self).__init__()
        self.affine1 = torch.nn.Linear(1, 128)
        self.linear = torch.nn.Linear(128, 1)
    def forward(self, x):
        """
        forward 
        """
        x = torch.from_numpy(np.array([x])).float() # convert obs to Torch tensor
        x = F.relu(self.affine1(x))
        x = F.sigmoid(self.linear(x))
        return x

class pyCell(Cell):
	NN = policy()
	def __init__(self,env,class_name): #can go
		super().__init__(env,class_name)
		self.age = randrange(0,100)
		self.setup()
	def setup(self):
		NN_value = self.NN.forward(self.age)
		max_cycle_t = 20
		min_cycle_t = 10
		self.cycle_t = (max_cycle_t-min_cycle_t)*NN_value+min_cycle_t

class pyDomain(Domain):
	def __init__(self):
		super().__init__()
		self._repo = []
	def generate_agent(self,agent_name):
		agent_obj = pyCell(self,agent_name)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	

if __name__ == '__main__':
	begin = time.time()
	envObj = pyDomain()
	envObj.setup()
	envObj.episode()
	end = time.time()
	print("Simulation took {} seconds".format(end-begin))

