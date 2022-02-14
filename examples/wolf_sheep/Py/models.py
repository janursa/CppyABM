import pandas as pd
import time

"""
@page purelypython
"""
"""@package Python example of CppyABM
Twi classes of Sheep and Wolf are developed to simulate sheep and wolfs. GrassPatch is the specialized Patch class to address
the growth of grass. The problem is multiagent (more than one type of agent class) and multipatch (more than one agent in a patch is allowed).
WolfSheep class simulates the environment.
"""
import sys, os,pathlib
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'../../../','build'))
from cppyabm.binds import Env, Agent, Patch, space
import random
import numpy as np

class PARAMS:
	"""
	steps: Simulation steps
	width*height: The dimentions of the simulation domain
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
	sheep_gain_from_food=4 


class Sheep(Agent):
	"""
	A sheep that walks around, reproduces (asexually) and gets eaten.

	"""

	def __init__(self, class_name, env, energy=None):
		super().__init__(class_name, env)
		self.energy = energy
		self.living = True

	def random_move(self):
		"""
		Step one cell in any allowable direction.
		"""
		# Pick the next cell from the adjacent cells.
		dest = random.choice(self.get_patch().neighbors)
		# Now move:
		self.move(dest,True)
	def step(self):
		"""
		A model step. Move, then eat grass and reproduce.
		"""
		self.random_move()
		# Reduce energy
		self.energy -= 1

		# If there is grass available, eat it
		grass_patch = self.get_patch()
		if grass_patch.fully_grown:
			self.energy += PARAMS.sheep_gain_from_food
			grass_patch.fully_grown = False

		# Death
		if self.energy < 0:
			self.disappear = True
			self.living = False

		if self.living and random.random() < PARAMS.sheep_reproduce:
			# Create a new sheep:
			self.energy /= 2
			lamb =self.env.generate_agent('Sheep',sheep_energy=self.energy)
			dest = self.get_patch()
			self.env.place_agent(dest,lamb,True)


class Wolf(Agent):
	"""
	A wolf that walks around, reproduces (asexually) and eats sheep.
	"""

	def __init__(self, class_name, env, energy=None):
		super().__init__(class_name, env)
		self.energy = energy
	def random_move(self):
		"""
		Step one cell in any allowable direction.
		"""
		# Pick the next cell from the adjacent cells.
		dest = random.choice(self.get_patch().neighbors)
		# Now move:
		self.move(dest,True)
	def step(self):
		self.random_move()
		self.energy -= 1
		# If there are sheep present, eat one
		local_agents = self.get_patch().get_agents()
		sheep = [obj for obj in local_agents if obj.class_name == 'Sheep']
		if len(sheep) > 0:
			sheep_to_eat = random.choice(sheep)
			self.energy += PARAMS.wolf_gain_from_food
			# Kill the sheep
			sheep_to_eat.disappear = True

		# Death or reproduction
		if self.energy < 0:
			self.disappear = True
		else:
			if random.random() < PARAMS.wolf_reproduce:
				# Create a new wolf cub
				self.energy /= 2
				cub = self.env.generate_agent('Wolf',wolf_energy=self.energy)
				dest = self.get_patch()
				self.env.place_agent(dest, cub,True)


class GrassPatch(Patch):
	"""
	A patch of grass that grows at a fixed rate and it is eaten by sheep
	"""

	def __init__(self, env,mesh_item, fully_grown, countdown):
		"""
		Creates a new patch of grass

		Args:
			grown: (boolean) Whether the patch of grass is fully grown or not
			countdown: Time for the patch of grass to be fully grown again
		"""
		super().__init__(env,mesh_item)
		self.fully_grown = fully_grown
		self.countdown = countdown

	def step(self):
		if not self.fully_grown:
			if self.countdown <= 0:
				# Set as fully grown
				self.fully_grown = True
				self.countdown = PARAMS.grass_regrowth_time
			else:
				self.countdown -= 1
			

class WolfSheep(Env):
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

		# Create sheep:
		for i in range(PARAMS.initial_sheep):
			energy = random.randrange(2 * PARAMS.sheep_gain_from_food)
			a =self.generate_agent("Sheep",sheep_energy=energy)
			dest = random.choice(self.patches)
			self.place_agent(dest,a,True)
 

		# Create wolves
		for i in range(PARAMS.initial_wolves):
			energy = random.randrange(2 * PARAMS.wolf_gain_from_food)
			a =self.generate_agent("Wolf",wolf_energy=energy)
			dest = random.choice(self.patches)
			self.place_agent(dest,a,True)


	def generate_agent(self,agent_name,sheep_energy=None,wolf_energy=None):
		"""
		Extension of the original function to create agents
		"""
		if agent_name == 'Wolf':
			agent_obj = Wolf(self,agent_name,energy = wolf_energy)
		elif agent_name == 'Sheep':
			agent_obj = Sheep(self,agent_name, energy = sheep_energy)
		else:
			raise ValueError('It is not defined')

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
		patch_obj = GrassPatch(self,mesh_item,fully_grown=fully_grown,countdown=countdown)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj)
		return patch_obj
	def step(self):
		self.activate_random()
		self.step_patches()
		# collect data
		self.collect_output()
		self.update()
		
	def collect_output(self):
		counts = self.count_agents()
		for key,value in counts.items():
			self.data[key].append(value)
		usage = self.memory_usage()
		self.data['memory'].append(usage)

	def episode(self):
		"""
		Runs an episode of the simulation.
		"""
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


