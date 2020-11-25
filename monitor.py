import sys
sys.path.insert(1,'../realtime_visualization/')
from realtime import monitor
settings = {
    "Medium": {
            "graph_dir" : "./examples/domain.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },
    "tissue density": {
            "graph_dir" : "./examples/tissuedensity.csv",
            "graph_type" : 'scatter',
            "graph_size" : 800
           },
    # "protein conc": {
    #         "graph_dir" : "./examples/proteinconc.csv",
    #         "graph_type" : 'scatter',
    #         "graph_size" : 800
    #        },
    "Cell counts": {
            "graph_dir" : "./examples/cell_count.csv",
            "graph_type" : 'lines',
            'x-axis-moves': False
           },
   # "pH": {
		 #    "graph_dir" : "./build/outputs/pH.csv",
		 #    "graph_type" : 'lines',
		 #    'x-axis-moves': False
		 #   },
   #  "TGF": {
   #      "graph_dir" : "./build/outputs/TGF.csv",
   #      "graph_type" : 'lines',
   #      'x-axis-moves': False
   #     },
   #  "BMP": {
   #      "graph_dir" : "./build/outputs/BMP.csv",
   #      "graph_type" : 'lines',
   #      'x-axis-moves': False
   #     },
   #  "maturity": {
   #      "graph_dir" : "./build/outputs/maturity.csv",
   #      "graph_type" : 'lines',
   #      'x-axis-moves': False
   #     },
   #  "ALP": {
   #      "graph_dir" : "./build/outputs/ALP.csv",
   #      "graph_type" : 'lines',
   #      'x-axis-moves': False
   #     },
   #  "OC": {
   #      "graph_dir" : "./build/outputs/OC.csv",
   #      "graph_type" : 'lines',
   #      'x-axis-moves': False
   #     }

}
monitor.watch(settings).run()