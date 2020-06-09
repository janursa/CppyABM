import sys
import random
sys.path.insert(1,"/Users/matin/Downloads/testProjs/env_agent/build/src")
from binds import Agent,Env,mesh_tools,Patch




class myPatch(Patch):
	def __init__(self,env):
		super().__init__(env)
	def step(self):
		pass


import time
class MSC(Agent):
	def __init__(self,env):
		super().__init__(env = env, class_name = 'MSC')

		self.storage = []
	def inherit(self,father):
		self.storage = father.storage

	def step(self):
		dest_patch = self.patch.empty_neighbor(quiet = True)
		self.storage.append(2)
		# self.order_hatch(patch = dest_patch, quiet = True, reset = True)
		self.order_move()
		# self.env.place_agent(dest_patch,new_agent)
		# self.storage.append(2)
		# 
		

		pass 
	


class myEnv(Env):
	
	def __init__(self):
		super().__init__()
		self.tick = 0
		self._repo = []
	def generate_patch(self):
		patch_obj = myPatch(self)
		self._repo.append(patch_obj)
		return patch_obj
	def generate_agent(self,agent_name):
		if agent_name == 'MSC':
			agent_obj = MSC(self)
		else:
			agent_obj = MSC(self)
			pass
		self._repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj

	def setup(self):
		mesh =  mesh_tools.grid(1,1,.1)
		self.setup_domain(mesh)
		
		for i in range(1):
			obj = self.generate_agent(MSC)
			self.place_agent_randomly(obj)
	def step(self):
		print('tick {}: agent counts {}'.format(self.tick,len(self.agents)))
		self.step_agents()	
		self.update()
		self.tick += 1
	def update(self):
		print("update")
		super().update()
		print("number of agents ",len(self.agents))
		
		self.log()
		time.sleep(1)
		if len(self.agents) >  len(self.patches):
			print("Agents cannot exceed patches in number")
			sys.exit(0)



	def log(self):
		x = []
		y = []
		type_ = []
		size_ = []
		file = open('scatter.csv','w')
		file.write('x,y,type,size\n')
		for index,patch in self.patches.items():
			if patch.empty == False:
				size_ = 10
				type_ = 'agent'
			else:
				size_ = 2
				type_ = 'nothing'
			file.write("{},{},{},{}\n".format(patch.coords[0],
											patch.coords[1],
											type_,
											size_))
		file.close()




# 		pass
# 	def step(self):
# 		self.step_agents()
# 		self.step_patches()
	

# 	
# 	def find_empty_patches(self,agent): # TODO
# 		pass
# 	def find_empty_patch(self): #TODO
# 		pass
# 	def find_empty_patch(self, agent): #TODO
# 		pass
# 	def place_agent(self,patch,agent): #TODO
# 		pass 
# 	def step_agents(self): #TODO
# 		pass 
# 	def step_patchs(self): #TODO
# 		pass
# 	def update_repos(self): #TODO
# 		pass 
# 	def move_agent(self,agent,patch): #TODO
# 		pass 
obj = myEnv()
obj.setup()
for i in range(20):
	obj.step()
obj.log()
# print(len(obj._repo))


# env_obj = myEnv()

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

