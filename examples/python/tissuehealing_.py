import numpy as np
from random import randrange
import sys
import os
import platform
import pathlib
import pandas as pd

current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'..','..','build'))
from binds.cppyabm import Env, Agent, Patch, grid2

class Tissue(Patch):
	def __init__(self,env):
		Patch.__init__(self,env)
		pass

class Cell (Agent):
	def __init__(self,env,agent_name):
		Agent.__init__(self,env = env,class_name = agent_name)
	def step(self):
		self.order_hatch(quiet=True)

class Domain(Env):
	def __init__(self):
		Env.__init__(self)
		self._repo = []
		self.clock = 0

	def generate_agent(self,agent_name):
		agent_obj = Cell(self,agent_name)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self):
		patch_obj = Tissue(self)
		self._repo.append(patch_obj)
		return patch_obj

	def setup(self):
		## create mesh
		length = .15 # mm	
		width = length #mm 	
		mesh = grid2(length=length, width=width, mesh_length=0.015, share = True)
		## create patch based on mesh
		self.setup_domain(mesh)
		## assiging 1000 cells randomly in the domain
		self.setup_agents({'cell':20})
		
		## create the damage
		self.update()
	def step(self):
		# execute cells
		# for cell in self.agents:
		# 	cell.step()
		self.update()
		self.clock +=1
	def episode(self):
		# self.update()
		# self.update()
		

		# print('taken patch # {} agent {}'.format(taken_patch,len(self.agents)))
		# taken_patch = 0
		# for [index,patch] in self.patches.items():
		# 	if patch.empty == False:
		# 		taken_patch+=1

		for i in range(20):
			taken_patch = 0
			for [index,patch] in self.patches.items():
				if patch.empty == False:
					taken_patch+=1
			print('Iteration {} taken patch # {} agent {}'.format(i,taken_patch,len(self.agents)))
			self.step()
			if i%20 ==0:
				self.output()
	def update(self):
		super().update()


	def output(self):
		# plot agents on the domain
		file = open('cells.csv','w')
		file.write('x,y,type,size\n')
		for agent in self.agents:
			x,y,z = agent.patch.coords
			file.write("{},{},{},{}\n".format(x, y, agent.class_name, 10))
		file.close()
		#plot ECM density on the domain
		# file = open('ECM.csv','w')
		# file.write('x,y,type,size\n')
		# for [index,patch] in self.patches.items():
		# 	x,y,z = patch.coords
		# 	file.write("{},{},{},{}\n".format(x, y, patch.ECM, 10))
		# file.close()
		# ## cell counts
		# df = pd.DataFrame.from_dict(self.data)
		# df_agent_counts = df[['cell_count']]
		# df_agent_counts.to_csv('cell_count.csv')

		


envObj = Domain()
envObj.setup()
envObj.episode()



