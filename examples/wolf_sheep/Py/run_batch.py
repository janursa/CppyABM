import os
import sys
file_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0,os.path.join(file_dir,'../../../scripts/'))

from batch_runner import Runner
from models import WolfSheep
import time
import pandas as pd
class SETTINGS:
	iter_n = 20
	output_dir = 'results_1.csv'


if __name__ == '__main__':
	begin = time.time()
	runner_obj = Runner(WolfSheep, iter_n=SETTINGS.iter_n, parallel=True)
	outputs = runner_obj.run()
	outputs = pd.DataFrame(outputs)
	outputs.to_csv(SETTINGS.output_dir)
	end = time.time()
	print('Running completed in {} seconds'.format(end-begin))

