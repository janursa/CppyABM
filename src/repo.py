## hatch update
for i in range(len(self.agents)):
	if self.agents[i].hatch_flag:
		try:
			dest = self.agents[i].patch.empty_neighbor()
		except:
			continue
		obj = self.generate_agent('MSC')
		self.place_agent(dest,obj)
