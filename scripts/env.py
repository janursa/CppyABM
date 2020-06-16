import sys
import random
import time
import pathlib
import os
import json
import pandas as pd
from pprogress import ProgressBar

current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,current_file_path)
sys.path.insert(1,os.path.join(current_file_path,'..','build','binds'))

from CPPYABM import Env,mesh_tools
from agents import MSC,Dead
from patch import myPatch

SETTINGS_PATH = os.path.join(current_file_path,'settings.json')
PARAMS_PATH = os.path.join(current_file_path,'params.json')
class myEnv(Env):
	def __init__(self,free_params = {}):
		super().__init__()
		## default fields
		self._repo = []
		self.data = {}
		## env variables
		self.tick = 0
		self.last_tick = 0
		## simulation specific
		with open(SETTINGS_PATH) as file:
			self.settings = json.load(file)
		with open(PARAMS_PATH) as file:
			self.params = json.load(file)
	def update_repo(self):
		self._repo[:]= [agent for agent in self._repo if not agent.disappear]
	def test_abstract(self):
		print("inside test abstract in base")
	def generate_patch(self):
		patch_obj = myPatch(self, configs = self.settings["setup"]["patch"].copy(),
								  params = self.params.copy())
		self._repo.append(patch_obj)
		return patch_obj
	def generate_agent(self,agent_name):
		if agent_name == 'MSC':
			agent_obj = MSC(self, configs = self.settings["setup"]["agents"]["MSC"].copy(),
								  params = self.params.copy())
		elif agent_name == 'Dead':
			agent_obj = Dead(self, configs = self.settings["setup"]["agents"]["Dead"].copy(),
								  params = self.params.copy())
		else:
			print("Generate agent is not defined for '{}'".format(agent_name))
			sys.exit(0)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj

	def setup(self):
		grid_info = self.settings["setup"]["grid"]
		mesh =  mesh_tools.grid(grid_info["x_l"],grid_info["y_l"],grid_info["patch_size"])
		self.setup_domain(mesh)
		## create agents
		agent_counts = self.settings["setup"]["agents"]["n"]
		self.setup_agents(agent_counts)
		self.update()
	def step(self):

		self.step_patches()
		self.step_agents()	

		self.tick += 1
		self.update()
		pass
	def update(self):
		super().update()
		## Either updates or appends a pair of key-value to self.data
		def add(key,value): 
			if key not in self.data:
				self.data[key] = [value]
			elif self.tick > self.last_tick:
			 	self.data[key].append(value)
			else:
				self.data[key][-1] = value
		## agent counts
		counts = self.count_agents()
		for key,count in counts.items():
			add (key,count)
		## average ph on patches
		pH_mean = self.collect_from_patches("pH")/len(self.patches)
		add("pH",pH_mean)
		## increment the updated tick
		self.last_tick = self.tick	
		## postprocessing 
		self.postprocessing()	
		## control
		if len(self.agents) >  len(self.patches):
			print("Agents cannot exceed patches in number")
			sys.exit(0)

	def episode(self):
		self.duration = self.settings["setup"]["exp_duration"]
		self.pb = ProgressBar(self.duration)
		for i in range(self.duration):
			self.step()
			self.pb.update()
		self.pb.done()
		print("Episode finished")
	def postprocessing(self):
		"""
		Post processing: logging the results to a file
		"""
		# agents on patches as scatter format
		# file = open('scatter.csv','w')
		# file.write('x,y,type,size\n')
		# for index,patch in self.patches.items():
		# 	if patch.empty:
		# 		size_ = 2
		# 		type_ = 'nothing'
		# 	else:
		# 		size_ = 10
		# 		type_ = patch.agent.class_name
				
		# 	file.write("{},{},{},{}\n".format(patch.coords[0],
		# 									patch.coords[1],
		# 									type_,
		# 									size_))
		# file.close()

		## agent counts 
		df = pd.DataFrame.from_dict(self.data)
		df_agent_counts = df[["MSC","Dead"]]
		df_agent_counts.to_csv('outputs/agents_traj.csv')
		## average pH 
		df_pH = df[["pH"]]
		df_pH.to_csv('outputs/pH.csv')
		pass
	def collect_from_patches(self,tag):
		sum_ = 0
		for index,patch in self.patches.items():
			sum_+=patch.data[tag]
		return sum_

