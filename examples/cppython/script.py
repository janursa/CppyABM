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
class pyCell(Cell):
	def __init__(self,env,class_name):
		super().__init__(env,class_name)
	def step(self):
		neighbor_cell_count = len(self.patch.find_neighbor_agents())
		# migration
		if neighbor_cell_count <= 8:
			self.order_move(quiet=True)
		# proliferation
		if self.patch.damage_center and self.clock >= 12:
			if neighbor_cell_count <= 6:
				self.order_hatch(quiet=True)
				self.clock = 0 
		# deposit ECM
		if self.patch.ECM < 100:
			self.patch.ECM += 1	
		# die
		if neighbor_cell_count >7:
			self.disappear = True
class pyDomain(Domain):
	def __init__(self):
		super().__init__()
		self._repo = []
	def generate_agent(self,agent_name):
		agent_obj = pyCell(self,agent_name)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def update(self):
		super().update()
		self.output()

	def output(self):
		# plot agents on the domain
		file = open('domain.csv','w')
		file.write('x,y,type,size\n')
		for agent in self.agents:
			x,y,z = agent.patch.coords
			file.write("{},{},{},{}\n".format(x, y, agent.class_name, 10))
		file.close()
		#plot ECM density on the domain
		# file = open('ECMdensity.csv','w')
		# file.write('x,y,type,size\n')
		# for [index,patch] in self.patches.items():
		# 	x,y,z = patch.coords
		# 	file.write("{},{},{},{}\n".format(x, y, patch.ECM, 10))
		# file.close()
		# ## cell counts
		# df = pd.DataFrame.from_dict(self.data)
		# df_agent_counts = df[['cell_count']]
		# df_agent_counts.to_csv('cell_count.csv')


if __name__ == '__main__':
	
	begin = time.time()
	envObj = pyDomain()
	envObj.setup()
	envObj.episode()
	end = time.time()
	print("Simulation took {} seconds".format(end-begin))

