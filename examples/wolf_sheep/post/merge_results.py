import os
import sys
import pandas as pd

class SETTINGS:
	# base_file_name = 'memory_'
	base_file_name = 'cpu_'
	# base_file_name = 'results_'
	iter_n = 20
	format_name = '.csv'
	# output_file_name = 'memorys.csv'
	output_file_name = 'cpus.csv'
	# output_file_name = 'resultss.csv'

if __name__ == "__main__":
	results_stack = []
	for i in range(SETTINGS.iter_n):
		file_name = SETTINGS.base_file_name+str(i)+SETTINGS.format_name
		data = pd.read_csv(file_name)
		results_stack.append(data)

	merged_results = pd.concat(results_stack, ignore_index=True)
	
	merged_results.to_csv(SETTINGS.output_file_name)