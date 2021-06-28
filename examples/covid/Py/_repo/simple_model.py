import time
import numpy as np
import random
import sys, os,pathlib
import matplotlib.pyplot as plt
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'../../..','build'))
from cppyabm.binds import Env, Agent, Patch, space

class InfectionModel(Env):
	"""A model for infection spread."""

	def __init__(self, N=10, width=10, height=10, ptrans=0.5,
				 death_rate=0.02, recovery_days=21,
				 recovery_sd=7,initial_infected_n = 1):
		Env.__init__(self)

		self.tick = 0
		self.patches_repo = []
		self.agents_repo = []
		self.data = {'susceptible':[],'infected':[],'removed':[]}
		self.num_agents = N
		self.recovery_days = recovery_days
		self.recovery_sd = recovery_sd
		self.ptrans = ptrans
		self.death_rate = death_rate
		mesh = space.grid2(length=height, width=width, mesh_length=1, share = True)
		## create patch based on mesh
		self.setup_domain(mesh)

		# Create agents
		for i in range(self.num_agents):
			person = self.generate_agent("Person")
			dest = random.choice(self.patches)
			self.place_agent(dest,person,True)

		#make some agents infected at start
		
		for i in range(initial_infected_n):
			victom = random.choice(self.agents)
			victom.state = State.INFECTED
			victom.recovery_time = self.get_recovery_time()


	def generate_agent(self,agent_name):
		"""
		Extension of the original function to create agents
		"""
		agent_obj = Person(self,agent_name)
		self.agents_repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self,mesh_item):
		"""
		Extension of the original function to create pacthes
		"""
		patch_obj = Patch(self,mesh_item)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj)
		return patch_obj
	def get_recovery_time(self):
		return int(np.random.normal(self.recovery_days,self.recovery_sd))

	def step(self):
		self.activate_random()
		self.update()
		self.tick +=1
	def episode(self):
		"""
		Simulate the whole duration of the run
		"""
		for i in range(100):
			print('Iteration {} cell count {}'.format(i,len(self.agents)))
			self.step()
		print(self.data['susceptible'])
		plt.plot(self.data['susceptible'])
		plt.ylabel('some numbers')
		plt.savefig('results.svg')
	def update(self):
		"""
		Update the model. A call to parent function is sent to take care of default functions.
		"""
		super().update()
		counts = dict(susceptible = 0, infected = 0, removed = 0)
		for agent in self.agents:
			if agent.state == State.SUSCEPTIBLE:
				counts['susceptible']+=1
			elif agent.state == State.INFECTED:
				counts['infected']+=1
			else:
				counts['removed']+=1
		self.data['susceptible'].append(counts['susceptible'])
		self.data['infected'].append(counts['infected'])	
		self.data['removed'].append(counts['removed'])	
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
class State:
	SUSCEPTIBLE = 0
	INFECTED = 1
	REMOVED = 2

class Person(Agent):
	""" An agent in an epidemic model."""
	def __init__(self, env, agent_name):
		super().__init__(env, agent_name)
		self.age = random.randint(20,40)        
		self.state = State.SUSCEPTIBLE  
		self.infection_time = 0


	def status(self):
		"""Check infection status"""

		if self.state == State.INFECTED:     
			drate = self.env.death_rate
			alive = np.random.choice([0,1], p=[drate,1-drate])
			# if alive == 0:
			#     self.disappear = True         
			t = self.env.tick-self.infection_time
			if t >= self.recovery_time:          
				self.state = State.REMOVED

	def contact(self):
		"""Find close contacts and infect"""

		cellmates = self.get_patch().get_agents()       
		if len(cellmates) > 1:
			for other in cellmates:
				if random.random() > self.env.ptrans:
					continue
				if self.state is State.INFECTED and other.state is State.SUSCEPTIBLE:                    
					other.state = State.INFECTED
					other.infection_time = self.env.tick
					other.recovery_time = model.get_recovery_time()

	def step(self):
		self.status()
		dest = random.choice(self.get_patch().neighbors)
		self.move(dest,True)
		self.contact()

if __name__ == '__main__':
	model = InfectionModel(N = 1000,width = 10, height = 10)
	model.episode()