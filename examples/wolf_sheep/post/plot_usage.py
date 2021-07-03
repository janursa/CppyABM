import plotly.graph_objs as go
import plotly.express as px
import numpy as np
import pandas as pd
from plotly.subplots import make_subplots
from copy import deepcopy

class SETTINGS:
    cpu_file_names = ['cpus_cpp_004.csv','cpus_py_004.csv','cpus_cppy_004.csv']
    memory_file_names = ['memorys_cpp_004.csv','memorys_py_004.csv','memorys_cppy_004.csv']
    # xs = [1,2,3,4]
    xs = [0,1,2]
    # postfixes = ['Cpp','Py','Cppy','Pyy']
    labels = ['Cpp', 'Py','Cppy']

    line_types = ['solid', 'dot', 'dash', 'longdash', 'dashdot', 'longdashdot']
    colors = ['DarkSlateGrey','black','DarkViolet']
    markers = ['circle','cross','diamond']
    fig_name = "usages.csv"
    ytitle = 'Megabytes/Seconds'
    xtitle = 'Model'


def plot(fig,stack_data,i,name,x):
    means = []
    max_stds = []    
    min_stds = []    
    for data in stack_data:
        mean_value = np.mean(data)
        std_upp = max(data) - mean_value
        std_down = mean_value - min(data)
        means.append(mean_value)
        max_stds.append(std_upp)
        min_stds.append(std_down)
    fig.add_trace(go.Scatter(
        # x=x,
        y=means,
        name = name,
        error_y=dict(
            type='data',
            symmetric=False,
            array=max_stds,
            arrayminus=min_stds,
            width = 10,
            thickness = 4
        ),
        mode='markers',
        marker = dict(size=10,
                color=SETTINGS.colors[i]),
        marker_symbol = SETTINGS.markers[i]
        )
        # line = dict(width=3, dash=line_types[0],color =colors[0] )
    )
    return fig


def update_layout(fig):
    fig.update_layout(
        width=450, #TODO: to go
        height=500,#TODO: to go
        xaxis = dict(
                title = SETTINGS.xtitle,
                titlefont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                ),
                showgrid=True,
                mirror=True,
                showline=True,
                # zeroline = False,
                linecolor = 'black',
                gridwidth = 20,
                tickfont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                )
            ),
        yaxis = dict(
                title = SETTINGS.ytitle,
                titlefont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                ),
                showgrid=True,
                mirror=True,
                showline=True,
                # zeroline = False,
                linecolor = 'black',
                gridwidth = 20,
                tickfont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                )
            ),
        

        paper_bgcolor='rgba(0,0,0,0)',
        plot_bgcolor='rgba(0,0,0,0)',
        legend=dict(
            orientation="h",
            x=0,
            y=1.2,
            font=dict(
                family='Times New Roman',
                size=30,
                color='#000'
            ),
            bordercolor='DarkSeaGreen' 
            # borderwidth=1
        ),
        margin=dict(
            l=50,
            r=50,
            b=100,
            t=100,
            pad=4
        )
    )

    fig.update_yaxes(
                automargin=True,
                showgrid=True,
                mirror=True,
                showline=True,
                linecolor = 'black',
                gridwidth = 20,
                dtick=30,
                tickfont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                ),
                zeroline = False)
    return fig



def change_unit(data,type_t):
    if type_t == 'CPU':
        return data/1000
    if type_t == 'Memory':
        return data/1000000



if __name__ == '__main__':

    stack_memory=[]
    stack_cpu=[]
    for i in range(len(SETTINGS.labels)):
        label = SETTINGS.labels[i]
        cpu_file_name = SETTINGS.cpu_file_names[i]
        memory_file_name = SETTINGS.memory_file_names[i]
        cpu_data = pd.read_csv(cpu_file_name)
        memory_data = pd.read_csv(memory_file_name)
        if label == 'Cpp':
            cpu_data['CPU'] = change_unit(cpu_data['CPU'],'CPU')
        stack_cpu.append(cpu_data['CPU'])
        stack_memory.append(change_unit(memory_data['Memory'],'Memory'))
    
    fig = go.Figure()
    fig = plot(fig,stack_memory,i = 0,name = 'Memory',x=SETTINGS.xs)
    fig = plot(fig,stack_cpu,i = 2,name = 'CPU',x=[x-0.2 for x in SETTINGS.xs])
    fig = update_layout(fig)
    fig.update_xaxes(automargin=True,showgrid=False,zeroline=False,
        tickvals =SETTINGS.xs,ticktext = SETTINGS.labels, range = [SETTINGS.xs[0]-0.5,SETTINGS.xs[-1]+0.5])
    # fig.update_yaxes(range = [0,220])
    fig.write_image(SETTINGS.fig_name+'.svg')
    
