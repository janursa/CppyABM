
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
    "ECM": {
            "graph_dir" : "cells.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },

}
monitor.watch(settings).run()