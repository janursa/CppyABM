import pandas as pd
import time
import random
import numpy as np
"""
@page purelypython
"""
"""@package Python example of CppyABM
We define two simple classes of Cell and Tissue as extensions of Agent and Patch, respectively,
to model cellular behavior and the properties of tissue, respectively.
"""
import sys, os,pathlib
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'build'))
from myBinds import Animal, GrassPatch, myEnv, space


class PARAMS:
	"""
	Args:
		initial_sheep: Number of sheep to start with
		initial_wolves: Number of wolves to start with
		sheep_reproduce: Probability of each sheep reproducing each step
		wolf_reproduce: Probability of each wolf reproducing each step
		wolf_gain_from_food: Energy a wolf gains from eating a sheep
		grass_regrowth_time: How long it takes for a grass patch to regrow
							 once it is eaten
		sheep_gain_from_food: Energy sheep gain from grass, if enabled.
	"""
	steps = 1000
	height=100
	width=100
	initial_sheep=2000
	initial_wolves=500
	# sheep_reproduce=0.04
	sheep_reproduce=0.08
	wolf_reproduce=0.05
	wolf_gain_from_food=40
	grass_regrowth_time=30
	sheep_gain_from_food=4 # case 1
	# sheep_gain_from_food=10 # case 2
			

class WolfSheep(myEnv):
	"""
	Wolf-Sheep Predation Model
	"""

	def __init__(self):
		super().__init__()
	
	def reset(self,iter_i):
		self.iter_i = iter_i
		self.agents.clear()
		self.patches.clear()
		self.agents_repo = []
		self.patches_repo = []
		mesh = space.grid2(PARAMS.height, PARAMS.width,1, True)
		self.setup_domain(mesh)
		self.data = {'Wolf':[],'Sheep':[],'memory':[]} 
		self.setup_animals("Sheep")
		self.setup_animals("Wolf")
	def setup_animals(self,class_name):
		sheep_energy = 0
		wolf_energy = 0
		if class_name == "Wolf":
			wolf_energy = random.randrange(2 * PARAMS.wolf_gain_from_food)
			number = PARAMS.initial_wolves
		
		elif (class_name == "Sheep"):
			sheep_energy = random.randrange(2 * PARAMS.sheep_gain_from_food)
			number = PARAMS.initial_sheep
		
		else: 
			raise ValueError();
		
		for i in range(number):
			a =self.generate_agent(class_name,wolf_energy=wolf_energy,sheep_energy=sheep_energy)
			dest = random.choice(self.patches)
			self.place_agent(dest,a,True)

	def generate_agent(self,agent_name,sheep_energy=None,wolf_energy=None):
		"""
		Extension of the original function to create agents
		"""
		if agent_name == "Wolf":
			agent_obj = Animal(self,agent_name,wolf_energy)
		elif agent_name == "Sheep":
			agent_obj = Animal(self,agent_name,sheep_energy)
		else:
			raise ValueError()

		self.agents_repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj

	def generate_patch(self,mesh_item):
		"""
		Extension of the original function to create pacthes
		"""
		fully_grown = random.choice([True, False])

		if fully_grown:
			countdown = PARAMS.grass_regrowth_time
		else:
			countdown = random.randrange(PARAMS.grass_regrowth_time)
		patch_obj = GrassPatch(self,mesh_item)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj)
		return patch_obj
	def step(self):
		# self.activate_random()
		for i in range(len(self.agents)):
			self.agents[i].step()
		self.step_patches()
		self.collect_output()
		self.update()

	def collect_output(self):
		counts = self.count_agents()
		for key,value in counts.items():
			self.data[key].append(value)
		usage = self.memory_usage()
		self.data['memory'].append(usage)

	def episode(self):
		begin = time.time()
		for i in range(PARAMS.steps):
			self.step()
		end = time.time()
		cpu = end-begin
		## output cpu
		cpu = pd.DataFrame({'CPU':[cpu]})
		cpu.to_csv('cpu_'+str(self.iter_i)+'.csv')
		## output memory
		memory = pd.DataFrame({'Memory':[np.mean(self.data['memory'])]})
		memory.to_csv('memory_'+str(self.iter_i)+'.csv')
		## output the data frame
		results = pd.DataFrame(self.data)
		results.to_csv('results_'+str(self.iter_i)+'.csv')
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


