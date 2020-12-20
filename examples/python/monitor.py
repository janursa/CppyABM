import sys
from realtime import monitor
settings = {
    "Cells": {
            "graph_dir" : "./examples/python/cells.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },
    "ECM": {
            "graph_dir" : "./examples/python/ECM.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },

    "Cell count": {
            "graph_dir" : "./examples/python/cell_count.csv",
            "graph_type" : 'lines',
            'x-axis-moves': False
           }
}
monitor.watch(settings).run()