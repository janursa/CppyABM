from models import WolfSheep
import time
import pandas as pd


if __name__ == '__main__':
	begin = time.time()
	runner_obj = WolfSheep()
	outputs = runner_obj.episode()
	outputs = pd.DataFrame(outputs)
	outputs.to_csv('single_outputs.csv')
	end = time.time()
	print('Running completed in {} seconds'.format(end-begin))

