import sys
sys.path.insert(1,"/Users/matin/Downloads/testProjs/env_agent/build/src")
from binds import Base,Env,AbstractFactory

class Derived(Base):
	def name(self):
		print("derived")


class factory(AbstractFactory):
	def create(self):
		obj = Derived()
		return obj

env = Env()
f_ob = factory()
env.get(f_ob)