import sys
sys.path.insert(1,"/Users/matin/Downloads/testProjs/env_agent/build/src")
from binds import Agent,Env,mesh_tools
mm = mesh_tools()
mesh = mm.grid(1,1,.1)
class MSC(Agent):
	def __init__(self,env):
		super().__init__(env)
	def step(self):
		print("MSC")


class myEnv(Env):
	def __init__(self):
		super().__init__()
	def setup(self):
		pass

# class myFactory(Factory):
# 	agents = []
# 	def create_agent(self):
# 		agent_obj = MSC()
# 		self.agents.append(agent_obj)
# 		return agent_obj
# 	def update(self):
# 		count = len(self.agents)
# 		print("cout is ",count)
# 		self.agents[:] = [agent for agent in self.agents if not agent.disappear_flag]
# 		print("Lef agents count {}".format(len(self.agents)))

