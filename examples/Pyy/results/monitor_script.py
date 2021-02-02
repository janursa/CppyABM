import sys, os,pathlib
current_file_path = pathlib.Path(__file__).parent.absolute()
sys.path.insert(1,'/Users/matin/Downloads/testProjs/RTvisalize')
from realtime import monitor
settings = {
    "Cell count": {
            "graph_dir" : "cell_count.csv",
            "graph_type" : 'lines',
            "graph_size" : 800,
            'x-axis-moves': False
           },
    "ECM": {
            "graph_dir" : "ECM.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },
    "Cells": {
            "graph_dir" : "cells.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },

}
monitor.watch(settings).run()