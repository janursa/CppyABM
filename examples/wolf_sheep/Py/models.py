import pandas as pd
import time

"""
@page purelypython
"""
"""@package Python example of CppyABM
We define two simple classes of Cell and Tissue as extensions of Agent and Patch, respectively,
to model cellular behavior and the properties of tissue, respectively.
"""
import sys, os,pathlib
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'../../../','build'))
from cppyabm.binds import Env, Agent, Patch, space
import random
import numpy as np

class PARAMS:
	steps = 1000
	height=100
	width=100
	initial_sheep=2000
	initial_wolves=500
	sheep_reproduce=0.04
	wolf_reproduce=0.05
	wolf_gain_from_food=40
	grass_regrowth_time=30
	sheep_gain_from_food=4 # case 1
	# sheep_gain_from_food=10 # case 2


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
			self.energy += self.env.sheep_gain_from_food
			grass_patch.fully_grown = False

		# Death
		if self.energy < 0:
			self.disappear = True
			self.living = False

		if self.living and random.random() < self.env.sheep_reproduce:
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
			self.energy += self.env.wolf_gain_from_food

			# Kill the sheep
			sheep_to_eat.disappear = True

		# Death or reproduction
		if self.energy < 0:
			self.disappear = True
		else:
			if random.random() < self.env.wolf_reproduce:
				# Create a new wolf cub
				self.energy /= 2
				cub = self.env.generate_agent('Wolf',wolf_energy=self.energy)
				dest = self.get_patch()
				self.env.place_agent(dest, cub,True)


class GrassPatch(Patch):
	"""
	A patch of grass that grows at a fixed rate and it is eaten by sheep
	"""

	def __init__(self, mesh_item, env, fully_grown, countdown):
		"""
		Creates a new patch of grass

		Args:
			grown: (boolean) Whether the patch of grass is fully grown or not
			countdown: Time for the patch of grass to be fully grown again
		"""
		super().__init__(mesh_item, env)
		self.fully_grown = fully_grown
		self.countdown = countdown

	def step(self):
		if not self.fully_grown:
			if self.countdown <= 0:
				# Set as fully grown
				self.fully_grown = True
				self.countdown = self.env.grass_regrowth_time
			else:
				self.countdown -= 1
			

class WolfSheep(Env):
	"""
	Wolf-Sheep Predation Model
	"""

	def __init__(
		self,
		height=PARAMS.height,
		width=PARAMS.width,
		initial_sheep=PARAMS.initial_sheep,
		initial_wolves=PARAMS.initial_wolves,
		sheep_reproduce=PARAMS.sheep_reproduce,
		wolf_reproduce=PARAMS.wolf_reproduce,
		wolf_gain_from_food=PARAMS.wolf_gain_from_food,
		grass_regrowth_time=PARAMS.grass_regrowth_time,
		sheep_gain_from_food=PARAMS.sheep_gain_from_food,
	):
		"""
		Create a new Wolf-Sheep model with the given parameters.

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
		super().__init__()
		# Set parameters
		self.agents_repo = []
		self.patches_repo = []
		self.height = height
		self.width = width
		self.initial_sheep = initial_sheep
		self.initial_wolves = initial_wolves
		self.sheep_reproduce = sheep_reproduce
		self.wolf_reproduce = wolf_reproduce
		self.wolf_gain_from_food = wolf_gain_from_food
		self.grass_regrowth_time = grass_regrowth_time
		self.sheep_gain_from_food = sheep_gain_from_food
		mesh = space.grid2(self.height, self.width,1, True)
		self.setup_domain(mesh)
		self.data = {'Wolf':[],'Sheep':[],'memory':[]} 


		# Create sheep:
		for i in range(self.initial_sheep):
			energy = random.randrange(2 * self.sheep_gain_from_food)
			a =self.generate_agent("Sheep",sheep_energy=energy)
			dest = random.choice(self.patches)
			self.place_agent(dest,a,True)
 

		# Create wolves
		for i in range(self.initial_wolves):
			energy = random.randrange(2 * self.wolf_gain_from_food)
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
			countdown = self.grass_regrowth_time
		else:
			countdown = random.randrange(self.grass_regrowth_time)
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
		for i in range(PARAMS.steps):
			self.step()
		return self.data
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


