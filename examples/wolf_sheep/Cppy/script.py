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

import numpy as np
import torch
from pympler import asizeof

sys.path.insert(1,os.path.join(current_file_path,'..','Pyy'))
from policy import policy
policy_model = policy()
policy_model.load_state_dict(torch.load(os.path.join(current_file_path,'..','Pyy','policy.pt')))
policy_model.eval()
class pyCell(Cell):
	NN = policy_model
	def __init__(self,env,class_name): #can go
		super().__init__(env,class_name)
		self.age = randrange(0,100)
		self.setup()
	def setup(self):
		NN_value = pyCell.NN.forward(self.age)
		max_cycle_t = 20
		min_cycle_t = 10
		self.cycle_t = (max_cycle_t-min_cycle_t)*NN_value+min_cycle_t

class pyDomain(Domain):
	memory_usage_max = 0

	def __init__(self,output_flag):
		super().__init__(output_flag)
		self.agents_repo = []
	def generate_agent(self,agent_name):
		agent_obj = pyCell(self,agent_name)
		self.agents_repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def step(self):
		super().step()
		usage = self.memory_usage()
		if usage > pyDomain.memory_usage_max:
			pyDomain.memory_usage_max = usage

	def update_repo(self):
		"""
		Updates the repository to remove inactive agents
		"""
		indices = []
		for i in range(len(self.agents_repo)):
			if self.agents_repo[i].disappear==True:
				indices.append(i)
		for ele in sorted(indices, reverse = True):  
			del self.agents_repo[ele]
	
	
if __name__ == '__main__':
	output_flag = False
	if len(sys.argv)>1:
		output_flag = True
	begin = time.time()
	envObj = pyDomain(output_flag)
	envObj.setup()
	envObj.episode()
	end = time.time()
	print("Simulation took {} seconds".format(end-begin))
	# print("Memory load: {}".format(pyDomain.memory_usage_max))
	# file = open('memory.txt','w')
	# file.write('{}\n'.format(pyDomain.memory_usage_max))
	# file.close()

