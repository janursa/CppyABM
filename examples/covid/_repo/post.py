import json
import pandas as pd
import matplotlib.pyplot as plt
import sys
file_params = sys.argv[1]
import os
current_dir = os.getcwd()

# Read JSON file
with open(file_params) as f:
  params = json.load(f)

tool = sys.argv[2]
# tool = 'Mesa'
# tool = 'CppyABM'
outputs_sum = {}

for iter_n in range(params['ensemble']['runs']):
	output = pd.read_csv('{}/outputs_{}.csv'.format(tool,iter_n))
	plt.figure()

	for tag in output.keys():
		if tag == 'susceptible' or tag == 'Unnamed: 0':
			continue
		x = output[tag]
		plt.plot(x,label=tag)
	plt.legend()
	plt.savefig(tool+'/graph_{}.svg'.format(iter_n))
	if iter_n == 0:
	    for key in output.keys():
	        outputs_sum.update({key:[]})
	for key,value in output.items():
	    outputs_sum[key].append(value)

outputs_sum = pd.DataFrame(outputs_sum)
outputs_sum.to_csv('{}/outputs_sum.csv'.format(tool))
# tags = ['susceptible','symptomatics','exposed','asymptomatics']


