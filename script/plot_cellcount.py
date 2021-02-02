import plotly.graph_objs as go
import plotly.express as px
import numpy as np
import pandas as pd
line_types = ['solid', 'dot', 'dash', 'dashdot', 'dashdot', 'longdashdot']
colors = ['DarkSlateGrey','DarkSeaGreen','DarkViolet','red']
def lines(data,name,x_lims,y_lims):
    fig = go.Figure()
    i =0
    for key,value in data.items():
        fig.add_trace(go.Scatter(
            y=value,
            name=key,
            line = dict(width=3, dash=line_types[i],color =colors[i] )
        ))
        i+=1

    fig.update_layout(
        width=550, #TODO: to go
        height=500,#TODO: to go
        title=dict(
            y= 0.9,
            x= 0.5,
            xanchor= 'center',
            yanchor= 'top',
            font=dict(
                family='sans-serif',
                size=20,
                color='DarkSlateGrey'
            )),

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
                ),
                range= x_lims
            ),
        
        yaxis = dict(
                showgrid=True,
                mirror=True,
                showline=True,
                linecolor = 'black',
                gridwidth = 20,
                dtick=1000,#TODO: to go
                tickfont = dict(
                    family = 'Times New Roman',
                    size = 30,
                    color = 'black'
                ),
                zeroline = False, 
                # range =
                #     [min([min(data[key]) for key in data.keys()]) - 0.5,
                #      max([max(data[key]) for key in data.keys()]) + 0.5]),
                range = y_lims),#TODO: to go

        paper_bgcolor='rgba(0,0,0,0)',
        plot_bgcolor='rgba(0,0,0,0)',
        legend=dict(
            orientation="h",
            x=.03,
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
    
    fig.update_yaxes(automargin=True,showgrid=False,zeroline=False)
    fig.update_xaxes(automargin=True,showgrid=False,zeroline=False)
    fig.write_image(name+'.svg')

def process(postfixes,plot_name):
    def extract_data(postfix):
        data = pd.read_csv('cell_count_{}.csv'.format(postfix))
        # process to remove unwanted coloumn
        if "Unnamed: 0" in data.keys(): 
            data = data.drop("Unnamed: 0", axis=1)
        return data
    data = map(lambda postfix: extract_data(postfix),postfixes)
    # concatenate data sets
    data = pd.concat(data, axis=1)
    x_lims = [-.5,338]
    y_lims = [1000,5050]
    # change the column names
    data.columns = postfixes
    lines(data,plot_name,x_lims= x_lims,y_lims=y_lims)
    

if __name__ == '__main__':
    postfixes = ['Cpp','Py','Cppy','Pyy']
    process(postfixes,'all')
    postfixes = ['Cpp','Py']
    process(postfixes,'Cpp_Py')
    
    
