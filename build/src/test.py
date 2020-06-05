from example import Agent,Env
# class pyAgent(Agent):
# 	def step(self):
# 		print("scheisse")
# class pyEnv(Env):
# 	def init(self,agent):
# 		self.agent = agent
# 		self.agent.step()
# 	def step(self):
# 		self.agent.step()
# env_obj = Env(Agent())
# env_obj.step()
# env_obj = Env(Agent())
# env_obj.step()
# call_go(pyAgent())
# 
class agent(Agent):
	def step(self):
		self.aa = 20
agent_obj = agent()
env_obj = Env()


env_obj.init(agent_obj)
env_obj.step()