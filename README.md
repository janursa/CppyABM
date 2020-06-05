# Agent-based model (ABM) framework
The package is designed to provide a generic framework for ABM development in Python language. The package offloads the user from dealing with the complexities of ABM development and allows them to solely focus on defining agents' and patches' behaviors. 
## Getting started
### Quick start
`pip install --upgrade ABMpy`

Define the algorithm that maps inputs to outputs for an agent:
```py
class agent1_model:
    def forward(self):
        p_var1 = self.inputs['patch']['p_var1']+self.inputs['self']['a_var1']*0.1
        a_var1 = self.inputs['patch']['p_var1']-self.inputs['self']['a_var1']
        self.outputs = {'patch':{'p_var1':p_var1},'self':{'a_var1':a_var1}}

    inputs = {'patch':{'p_var1':0},'self':{'a_var1':0}}
    outputs = {'patch':{'p_var1':0},'self':{'a_var1':0}}
```
Do the same for as many agent types as is desired. Define the same for patch.

```py
class patch_model:
    def forward(self):
        map_input_to_output

    inputs = {some_inputs}
    outputs = {some_outputs}
```
Define settings for ABM:
```
settings = {
    "configs":{
      "exp_duration" : 72,
      "domain":{
          "x_l": 5,
          "y_l": 10,
          "patch_size": 0.5
      },
      "patch":{
          "attrs":{ "p_var1":12}
        },
      "agents":
        {
          "agent_type1":{
            "initial_n": 100,
            "attrs":{"a_var1":0.015}
          }
        }
    }
```
Run the ABM:
```
from ABM import ABM
obj = ABM({'agent_type1':agent1_model},patch_model,settings)
obj.run()
```

