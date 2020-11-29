import numpy as np
from random import randrange
import sys
import os
import platform
import pathlib
import pandas as pd

# current_file_path = pathlib.Path(__file__).parent.absolute()
# sys.path.insert(1,current_file_path)
# if platform.system() == 'Windows':
# 	sys.path.insert(1,os.path.join(current_file_path,'..','build','x64-Release'))
# else:
# 	sys.path.insert(1,os.path.join(current_file_path,'..','build'))
from binds.cppyabm import Env, Agent, Patch, grid2

class myPatch(Patch):
	def __init__(self,env):
		Patch.__init__(self,env)
		self.damage_center = False
		self.tissue = 100

class Cell (Agent):
	def __init__(self,env,agent_name):
		Agent.__init__(self,env = env,class_name = agent_name)
		self.clock = 12
	def update(self):
		self.clock+=1

class healingEnv(Env):
	def __init__(self):
		Env.__init__(self)
		self._repo = []
		self.clock = 0
		self.data = {'cell_count':[],'tissue_density':[]}
	def generate_agent(self,agent_name):
		agent_obj = Cell(self,agent_name)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self):
		patch_obj = myPatch(self)
		self._repo.append(patch_obj)
		return patch_obj
	def damage(self):
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			if (x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25):
				patch.damage_center = True
				patch.tissue = 0
				if patch.empty == False:
					patch.agent.disappear = True
	def setup(self):
		## create mesh
		length = 1.5 # mm	
		width = length #mm 	
		mesh = grid2(length=length, width=width, mesh_length=0.015, share = True)
		## create patch based on mesh
		self.setup_domain(mesh)
		## assiging 1000 cells randomly in the domain
		self.setup_agents({'cell':2000})
		## create the damage
		self.damage()
		self.update()
	def run(self):
		# execute cells
		for cell in self.agents:
			# dest_patch = cell.patch.empty_neighbor(quiet=False)
			cell.order_move(quiet=True)
			# proliferation
			neighbor_cell_count = len(cell.patch.find_neighbor_agents())
			if cell.patch.damage_center and cell.clock >= 12:
				if neighbor_cell_count <= 6:
					cell.order_hatch(quiet=True)
					cell.clock = 0 
			# deposit tissue
			if cell.patch.tissue < 100:
				cell.patch.tissue += 1	
			# die
			if neighbor_cell_count >7:
				cell.disappear = True
		self.update()
		self.clock +=1
	def update(self):
		super().update()
		for agent in self.agents:
			agent.update()
		cell_count = self.count_agents()
		self.data['cell_count'].append(cell_count['cell'])
		tissue_density_sum = 0
		for _,patch in self.patches.items():
			tissue_density_sum+=patch.tissue

		tissue_mean = tissue_density_sum/len(self.patches)
		self.data['tissue_density'].append(tissue_mean)

	def output(self):
		# plot agents on the domain
		file = open('domain.csv','w')
		file.write('x,y,type,size\n')
		for agent in self.agents:
			x,y,z = agent.patch.coords
			file.write("{},{},{},{}\n".format(x, y, agent.class_name, 10))
		file.close()
		#plot tissue density on the domain
		file = open('tissuedensity.csv','w')
		file.write('x,y,type,size\n')
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			file.write("{},{},{},{}\n".format(x, y, patch.tissue, 10))
		file.close()
		## cell counts
		df = pd.DataFrame.from_dict(self.data)
		df_agent_counts = df[['cell_count']]
		df_agent_counts.to_csv('cell_count.csv')

		


envObj = healingEnv()
envObj.setup()
import time
for i in range(336):
	print('Iteration {}'.format(i))
	envObj.run()
	
envObj.output()

