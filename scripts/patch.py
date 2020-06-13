import sys
import random
import pathlib
import os
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,current_file_path)
sys.path.insert(1,os.path.join(current_file_path,'..','build','binds'))
from binds import Patch




class myPatch(Patch):
	"""
	This class describes my patch.
	"""
	def __init__(self,env,configs = None, params = None):
		super().__init__(env)
		self.configs = configs or {}
		self.params = params or {}
		## initialize
		for key,value in self.configs['attrs'].items():
			self.data[key] = value;
	def step(self):
		pH_new = self.pH()
		new_lactate = self.lactate()
		self.data["pH"] = pH_new
		self.data["lactate"] = new_lactate
		self.data["agent_density"] = len(self.find_neighbor_agents(include_self = True))/9.0
		return
	def pH(self):
		mg = self.data["Mg"]
		lactate = self.data["lactate"]
		pH_new = self.params["w_mg_ph"]*mg -self.params["w_lactate_ph"]*lactate + 7.8;
		return pH_new
	def lactate(self):
		if not self.empty and self.agent.class_name != "Dead":
			MI = self.agent.data["MI"]
		else:
			MI = 0
		w = self.params["w_MI_lactate"]
		lactate = self.data["lactate"] + w * MI
		return lactate
