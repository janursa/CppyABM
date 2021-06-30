import sys, os,pathlib
from realtime import monitor
settings = {
    "Cell count": {
            "graph_dir" : "cell_count.csv",
            "graph_type" : 'lines',
            "col" : 'col s4',
            'x-axis-moves': False
           },
    "ECM": {
            "graph_dir" : "ECM.csv",
            "graph_type" : 'map',
            "col" : 'col s4',
            "range_color": [0,100]
           },
    "Cells": {
            "graph_dir" : "cells.csv",
            "graph_type" : 'scatter2',
            "col" : 'col s4',
           },

}
monitor.watch(settings).run()