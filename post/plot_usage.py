import plotly.graph_objs as go
import plotly.express as px
import numpy as np
import pandas as pd
from plotly.subplots import make_subplots
from copy import deepcopy

line_types = ['solid', 'dot', 'dash', 'longdash', 'dashdot', 'longdashdot']
colors = ['DarkSlateGrey','black','DarkViolet']
markers = ['circle','cross','diamond']
def plot(fig,data,i,name,x):
    means = []
    max_stds = []    
    min_stds = []    
    for tag in data.columns:
        tag_data = data[tag]
        mean_value = np.mean(tag_data)
        std_upp = max(tag_data) - mean_value
        std_down = mean_value - min(tag_data)
        means.append(mean_value)
        max_stds.append(std_upp)
        min_stds.append(std_down)
    fig.add_trace(go.Scatter(
        x=x,
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
                color=colors[i]),
        marker_symbol = markers[i]
        )
        # line = dict(width=3, dash=line_types[0],color =colors[0] )
    )
    return fig


def update_layout(fig):
    fig.update_layout(
        width=450, #TODO: to go
        height=500,#TODO: to go
        xaxis = dict(
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
def process(postfixes,fig_name, xs ):

    def extract_data(postfix):
        data = pd.read_csv('usage_{}.csv'.format(postfix))
        # process to remove unwanted coloumn
        if "Unnamed: 0" in data.keys(): 
            data = data.drop("Unnamed: 0", axis=1)
        return data
    data = map(lambda postfix: extract_data(postfix),postfixes)
    # memory_data = []
    # for item in list(data):
    #     memory_data.append()
    # memory_data = 
    memory_data = pd.concat([item['Memory'].div(10**6) for item in list(deepcopy(data))],axis=1)
    memory_data.columns = postfixes
    CPU_data = pd.concat([item['CPU'] for item in list(deepcopy(data))],axis=1)
    CPU_data.columns = postfixes
    fig = go.Figure()
    fig = plot(fig,memory_data,i = 0,name = 'Memory',x=xs)
    fig = plot(fig,CPU_data,i = 2,name = 'CPU',x=[x-0.2 for x in xs])
    fig = update_layout(fig)
    fig.update_xaxes(automargin=True,showgrid=False,zeroline=False,
        tickvals = xs,ticktext = postfixes, range = [xs[0]-0.5,xs[-1]+0.5])
    fig.update_yaxes(range = [0,220])
    fig.write_image(fig_name+'.svg')



if __name__ == '__main__':
    # xs = [1,2]
    # postfixes = ['Cpp','Py']
    # process(postfixes,'Cpp_Py',xs)
    # postfixes = ['Cppy','Pyy']
    # process(postfixes,'Cppy_Pyy',xs)
    xs = [1,2,3,4]
    postfixes = ['Cpp','Py','Cppy','Pyy']
    process(postfixes,'all',xs)
    
