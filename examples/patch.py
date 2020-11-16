import sys
import random
import pathlib
import os
current_file_path = pathlib.Path(__file__).parent.absolute()

from imports import *



class myPatch(Patch):
	"""
	This class describes my patch.
	"""
	def __init__(self,env,configs = None, params = None, id_ = None):
		super().__init__(env)
		self.configs = configs or {}
		self.params = params or {}
		self.data = {}
		self.id = id_
		## initialize
		for key,value in self.configs['attrs'].items():
			self.data[key] = value;
	def step(self):
		pH_new = self.pH()
		new_lactate = self.lactate()
		self.data["pH"] = pH_new
		self.data["lactate"] = new_lactate
		self.data["agent_density"] = len(self.find_neighbor_agents(include_self = True))/27
		return
	def pH(self):
		mg = self.data["Mg"]
		lactate = self.data["lactate"]
		pH_new = self.params["w_mg_ph"]*mg -self.params["w_lactate_ph"]*lactate + 7.8;
		# print("mg {} lactate {} pH_new {}".format(mg,lactate,pH_new))
		return pH_new
	def lactate(self):
		if not self.empty and self.agent.class_name != "Dead":
			MI = self.agent.data["MI"]
		else:
			MI = 0
		w = self.params["w_MI_lactate"]
		lactate = self.data["lactate"] + w * MI
		return lactate
	def get_data(self,tag):
		pass
	def set_data(self,tag):
		pass
