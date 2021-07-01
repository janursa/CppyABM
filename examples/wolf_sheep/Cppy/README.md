
# The implementation of Cppy model
The Cppy model extends the Cpp model. 
This folder contains:
- `binds.cpp`: This file creates Python bindings. 
- `script.py`: The extended model is written in this file. 
- `CmakeLists.txt`: Cmake file
- `monitor_script.py`: Visualization script. This file should be executed using `python3 monitor_script.py` in a folder that contains result files.

To execute the model after compilation,
- `python script.py`: execution without outputs
- `python script.py outout`: execution with outputs

The model includes the implementation file located in the Cpp folder and the policy model located in the Pyy folder.

To execute the visualization after outputs are available,
- `python monitor_script.py` within the folder that contains the outputs