import json
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import sys
import os
import numpy as np
current_dir = os.getcwd()



# tags =['Exposed','Asymptomatic','SymptQuarantined','AsymptQuarantined']
# tags =['Rt']
# tags = ['Susceptible']




class PARAMS:
	# tag = 'SymptQuarantined'
	tags = ['Sheep','Wolf']
	period_t = 100
	iter_n = 1
	data_file = 'single_run_results.csv'
	labels = ['Sheep','Wolf']
	# colors = ['green','blue','red','black']
	colors = ['green','blue']
	ytitle = 'Agent numbers'
	xtitle = 'Iteration'
	# graph_name = 'graph.svg'


class FONT:
	family = 'Times New Roman'
	legend = 18
	tick_font_size = 20
	axis_title_font_size = 20
	title_font_size = 20

def process_data(output):
	cumulated_data = [[] for i in range(PARAMS.period_t)]
	for i in range(PARAMS.period_t):
		for j in range(PARAMS.iter_n):
			cumulated_data[i].append(output[i+j*PARAMS.period_t])
	min_bounds = []
	max_bounds = []
	mean_bounds = []
	for vector in cumulated_data:
		min_bounds.append(min(vector))
		max_bounds.append(max(vector))
		mean_bounds.append(np.mean(vector))
	return min_bounds,max_bounds,mean_bounds



def update_layout(ax):

	ax.legend(bbox_to_anchor=(.9, .95),loc = 'upper right', borderaxespad=0.,prop={ 'family':FONT.family,'size':FONT.legend},ncol=1)
	
	# ax.set_ylim(yrange)
	# labels = [item.get_text() for item in ax.get_xticklabels()]
	# print(labels)
	# ax.set_xticks(ticks = [int(i) for i in x_labels])
	# ax.set_xticklabels(x_labels_adj)
	
	# ax.get_xaxis().set_major_formatter(
	#     matplotlib.ticker.FuncFormatter(lambda x, p: int(x*15/(60*24))))

	for label in (ax.get_xticklabels() + ax.get_yticklabels()):
		label.set_fontname(FONT.family)
		label.set_fontsize(FONT.tick_font_size)


	ax.set_ylabel(PARAMS.ytitle,fontdict ={'family':FONT.family,'size':FONT.axis_title_font_size})
	ax.set_xlabel(PARAMS.xtitle,fontdict ={'family':FONT.family,'size':FONT.axis_title_font_size})
	
	# ax.set_title('Title',fontdict ={'family':FONT.family,'size':FONT.title_font_size, 'fontweight':'bold'})
if __name__ == '__main__': 
	## read data
	
	output = pd.read_csv(PARAMS.data_file)
	## plot
	fig = plt.figure(figsize=(8,7))
	fig.canvas.draw()
	ax = fig.add_subplot(1, 1, 1)

	for i in range(len(PARAMS.tags)):
		label = PARAMS.labels[i]
		color = PARAMS.colors[i]
		min_bounds,max_bounds,mean_bounds = process_data(output[PARAMS.tags[i]])
		ax.fill_between(range(PARAMS.period_t), min_bounds, max_bounds,
	                 facecolor=color, # The fill color
	                 color=color,       # The outline color
	                 alpha=0.2)          # Transparency of the fill
		# gap = 0
		# for i in range(iter_n):
		# 	start = i*period_t +gap
		# 	end = (i+1)*period_t -gap
		# 	print("start {} end {}".format(start,end))
		# 	plt.plot(range(period_t-2*gap),output[tag][start:end])
			
		ax.plot(mean_bounds,label=label)
	update_layout(ax)
		# plt.plot(x,label=tag)
	# plt.legend()
	plt.savefig(os.path.join(current_dir,'{}.svg'.format(PARAMS.data_file)),bbox_inches="tight")
	


