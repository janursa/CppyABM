import sys
import random
import pathlib
import os
import time
import platform
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,current_file_path)
# sys.path.insert(1,os.path.join(current_file_path,'..','..','ABM_base','build','binds'))
if platform.system() == 'Windows':
	sys.path.insert(1,os.path.join(current_file_path,'..','build','x64-Release','binds'))
else:
	sys.path.insert(1,os.path.join(current_file_path,'..','build','binds'))
from CPPYABM import Env,grid,grid3, Patch, Agent

def update_progress(progress):
    barLength = 50 # Modify this to change the length of the progress bar
    status = ""
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
        status = "error: progress var must be float\r\n"
    if progress < 0:
        progress = 0
        status = "Halt...\r\n"
    if progress >= 1:
        progress = 1
        status = "Done...\r\n"
    block = int(round(barLength*progress))
    text = "\rPercent: [{0}] {1}% {2}".format( "#"*block + "-"*(barLength-block), (int)(progress*100), status)
    sys.stdout.write(text)
    sys.stdout.flush()