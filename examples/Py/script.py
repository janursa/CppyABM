import pandas as pd
import time
from pympler import asizeof

"""
@page purelypython
"""
"""@package Python example of CppyABM
We define two simple classes of Cell and Tissue as extensions of Agent and Patch, respectively,
to model cellular behavior and the properties of tissue, respectively.
"""
import sys, os,pathlib
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'..','..','build'))
from cppyabm.binds import Env, Agent, Patch, space

class Tissue(Patch):
	"""This class extends Patch to simulate tissue properties.
	"""
	def __init__(self,env,mesh_item):
		Patch.__init__(self,env,mesh_item)
		self.setup()
	def setup(self):
		self.damage_center = False # identifies the patch as part of the damaged area
		self.ECM = 100 # quantity of extracellular matrix
class Cell (Agent):
	"""
	This class extends Agent to simulate cells.
	"""
	def __init__(self,env,agent_name):
		Agent.__init__(self,env = env,class_name = agent_name)
		self.setup()
	def setup(self):
		self.clock = 0 
		self.cycle_t = 12 # clock required before cell commit proliferatoin
	def update(self):
		self.clock+=1
	def step(self):
		"""
		Simulates cellular reactions of migration, proliferation, tissue deposition, and death.
		"""
		# migration
		self.order_move(quiet=True)
		# proliferation
		neighbor_cell_count = len(self.patch.find_neighbor_agents())
		if self.patch.damage_center and self.clock >= self.cycle_t:
			if neighbor_cell_count <= 6:
				self.order_hatch(quiet=True)
				self.clock = 0 
		# ECM synthesize
		if self.patch.ECM < 100:
			self.patch.ECM += 1	
		# apoptosis
		if neighbor_cell_count >7:
			self.disappear = True
			

class Domain(Env):
	"""
	This class extends Env to simulate coordinate the simulation.
	"""
	memory_usage_max = 0
	def __init__(self,output_flag):
		Env.__init__(self)
		self.agents_repo = []
		self.patches_repo = []
		self.tick = 0
		self.data = {'cell_count':[]}
		self.output_flag = output_flag
		
	def generate_agent(self,agent_name):
		"""
		Extension of the original function to create agents
		"""
		agent_obj = Cell(self,agent_name)
		self.agents_repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self,mesh_item):
		"""
		Extension of the original function to create pacthes
		"""
		patch_obj = Tissue(self,mesh_item)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj)
		return patch_obj
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
	def damage(self):
		"""
		Create damage
		"""
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			if (x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25):
				patch.damage_center = True
				patch.ECM = 0
				if patch.empty() == False:
					patch.get_agent().disappear = True
	def setup(self):
		"""
		Setup the simulation by creating mesh, patches, damage, and agents
		"""
		## create mesh
		length = 1.5 # mm	
		width = length #mm 	
		mesh = space.grid2(length=length, width=width, mesh_length=0.015, share = True)
		## create patch based on mesh
		self.setup_domain(mesh)
		## assiging 2000 cells randomly in the domain
		self.setup_agents({'cell':2000})
		## create the damage
		self.damage()
		self.update()
	def step(self):
		"""
		Simulate the progress of the model
		"""
		# execute cells
		# h = hpy()
		usage = self.memory_usage()
		if usage>Domain.memory_usage_max:
			Domain.memory_usage_max = usage
		
		for cell in self.agents:
			cell.step()
		self.update()
		self.tick +=1
	def episode(self):
		"""
		Simulate the whole duration of the run
		"""
		for i in range(336):
			print('Iteration {} cell count {}'.format(i,len(self.agents)))
			self.step()
			if self.output_flag:
				self.output()
	def update(self):
		"""
		Update the model. A call to parent function is sent to take care of default functions.
		"""
		super().update()
		for agent in self.agents:
			agent.update()
		cell_count = self.count_agents()
		self.data['cell_count'].append(cell_count['cell'])

	def output(self):
		"""
		Output the results
		"""
		# plot agents on the domain
		file = open('cells.csv','w')
		file.write('x,y,type,size\n')
		for agent in self.agents:
			x,y,z = agent.patch.coords
			file.write("{},{},{},{}\n".format(x, y, agent.class_name, 10))
		file.close()
		#plot ECM density on the domain
		file = open('ECM.csv','w')
		file.write('x,y,type,size\n')
		for [index,patch] in self.patches.items():
			x,y,z = patch.coords
			file.write("{},{},{},{}\n".format(x, y, patch.ECM, 10))
		file.close()
		## cell counts
		df = pd.DataFrame.from_dict(self.data)
		df_agent_counts = df[['cell_count']]
		df_agent_counts.to_csv('cell_count.csv')

	
if __name__ == '__main__':
	output_flag = False
	if len(sys.argv) > 1:
		output_flag = True
	begin = time.time()
	envObj = Domain(output_flag)
	envObj.setup()
	envObj.episode()
	end = time.time()
	print("Simulation took {} seconds".format(end-begin))
	print("Memory load: {}".format(Domain.memory_usage_max))
	file = open('memory.txt','w')
	file.write('{}\n'.format(Domain.memory_usage_max))
	file.close()
	
