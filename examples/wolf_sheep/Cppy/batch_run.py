import os
import sys
file_dir = os.path.abspath(os.path.dirname(__file__))
sys.path.insert(0,os.path.join(file_dir,'../../../scripts/'))

# from batch_runner_serial import SerialBatchRunner
from batch_runner_parallel import ParallelBatchRunner
from models import WolfSheep
import time
import pandas as pd
import time
class SETTINGS:
	iter_n = 20


if __name__ == '__main__':
	begin = time.time()
	model = WolfSheep()
	# runner_obj = SerialBatchRunner(model, iter_n=SETTINGS.iter_n)
	runner_obj = ParallelBatchRunner(model, iter_n=SETTINGS.iter_n)
	runner_obj.run()
	end = time.time()
	print("The simulation took {} seconds".format(end-begin))
