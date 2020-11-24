import numpy as np
from random import randrange
import sys
import os
import platform
import pathlib

current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,current_file_path)
if platform.system() == 'Windows':
	sys.path.insert(1,os.path.join(current_file_path,'..','build','x64-Release'))
else:
	sys.path.insert(1,os.path.join(current_file_path,'..','build'))
from binds.cppyabm import Env, Agent, Patch, grid2

class grid(Patch):
	def __init__(self,env):
		Patch.__init__(self,env)
		self.damage_center = False
		self.has_cell = False
		self.tissue = 100

class cell (Agent):
	def __init__(self,env):
		Agent.__init__(self,env = env,class_name = 'cell')
		self.clock = 12

class healingEnv(Env):
	def __init__(self):
		Env.__init__(self)
		self._repo = []
		self.clock = 0
	def generate_agent(self,agent_name):
		agent_obj = cell(self)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self):
		patch_obj = grid(self)
		self._repo.append(patch_obj)
		return patch_obj
	def damage(self):
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			if (x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25):
				patch.damage_center = True
	def setup(self):
		## create mesh
		length = 1.5 # mm	
		width = length #mm 	
		mesh = grid2(length=length, width=width, mesh_length=0.015, share = False)
		## create patch based on mesh
		self.setup_domain(mesh)
		## assiging 1000 cells randomly in the domain
		self.setup_agents({'cell':1000})
		## assigining cells to the boundary patches
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			if patch.on_border and patch.empty:
				new_cell = self.generate_agent(self)
				self.connect_patch_agent(patch,new_cell)
	def run(self):
		if self.clock < 20:
			self.inflammation()
		# execute cells
		for cell in self.agents:
			# if gradient is significant migrate based on chemotaxis otherwise randomly
			max_gradient = max(gradients)
			if max_gradient >= 0.1:
				max_index = gradients.index(max_gradient)
				dest_patch = cell.patch.neighbors[max_index]
				cell.order_move(dest_patch,quiet = True)
			else:
				# rand_index = randrange(len(cell.patch.neighbors))
				# dest_patch = cell.patch.neighbors[rand_index]
				pass
			# cell.order_move(dest_patch,quiet = True)
			# proliferation
			if cell.clock == 24:
				cell.order_hatch(quiet = True)
			# deposit tissue
			cell.patch.tissue += 0.05
			if cell.patch.tissue > 1:
				cell.patch.tissue = 2
			# die
			if cell.patch.tissue == 1:
				cell.disappear = True
		self.update()
		self.clock +=1
	def update(self):
		super().update()

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

		


envObj = healingEnv()
envObj.setup()
import time
for i in range(100):
	print('Iteration {}'.format(i))
	# time.sleep(2)
	envObj.run()
	envObj.output()

