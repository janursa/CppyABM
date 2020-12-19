import numpy as np
from random import randrange
import sys
import os
import platform
import pathlib
import pandas as pd

current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,os.path.join(current_file_path,'build'))
from myBinds import healingEnv

class myhealingEnv(healingEnv):
	def generate_agent(self,agent_name):
		print('this egent')
		agent_obj = Cell(self,agent_name)
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj


envObj = myhealingEnv()
envObj.setup()
# for i in range(100):
# 	envObj.step()
