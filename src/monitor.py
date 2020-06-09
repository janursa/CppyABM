import sys
sys.path.insert(1,'/Users/matin/Downloads/testProjs/realtime_monitoring')
from realtime import monitor
settings = {
    "plot1": {
            "graph_dir" : "/Users/matin/Downloads/testProjs/env_agent/build/scatter.csv",
            "graph_type" : 'scatter',
            "graph_size" : 600
           }
}
monitor.watch(settings).run()