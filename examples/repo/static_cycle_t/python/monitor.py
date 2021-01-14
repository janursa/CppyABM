import sys
import os
import pathlib
from realtime import monitor
settings = {
    "Cells": {
            "graph_dir" : "cells.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },
    "ECM": {
            "graph_dir" : "ECM.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },

    "Cell count": {
            "graph_dir" : "cell_count.csv",
            "graph_type" : 'lines',
            'x-axis-moves': False
           }
}
monitor.watch(settings).run()