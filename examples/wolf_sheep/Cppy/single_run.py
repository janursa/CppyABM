import os
import sys
file_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0,os.path.join(file_dir,'../../../scripts/'))

from models import MyWolfSheep
import time
import pandas as pd
import time
import matplotlib.pyplot as plt


if __name__ == '__main__':
	begin = time.time()
	model = MyWolfSheep()
	model.reset(100)
	model.episode()
	results = pd.read_csv('results_100.csv')
	plt.figure()
	for tag in ['Wolf','Sheep']:
		data = results[tag]
		plt.plot(data,label= tag)
	plt.savefig('results_100.svg',bbox_inches="tight")

	end = time.time()
	print("The simulation took {} seconds".format(end-begin))
