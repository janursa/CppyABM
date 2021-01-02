
  

# General purpose agent-based modeling library

<a href="https://janursa.github.io/CppyABM/" title="About Me">About Me</a>

  
## Getting started

### Quick start

`pip install --upgrade cppyabm`

```py

# inside your script, e.g. test.py

from ABC import tools

obj = tools.ABC(settings = settings, free_params = free_params)

obj.sample()

obj.run()

obj.postprocess()

```

```py

# in terminal

mpiexec -n available_cpu_core python test.py

# or

mpiexec -n available_cpu_threads --use-hwthread-cpus python test.py

```

### More on it

ABC module receives two inputs from users.  First, the free parameters' list that is a python dictionary that contains the names and bounds (min and max) of each free parameter, as shown below:

```python
free_params = {
    'p_name_1': [1.1,4.3], # [min,max]
    'p_name_2': [6.4,23.1]
}
```
Second, the settings that is another python dictionary that contains user-specific information:

```py
settings = {
    "MPI_flag": True, # Use of MPI
    "sample_n": 10000,  # Sample number
    "top_n": 100, # Number of top selected samples, i.e. posterior
    "output_path": "outputs", # Relative output directory to save the results
    "run_func":custom_func, # A custom function that calculates the error for a given dataset
    "args":{  # Optional arguments that `custom_func` requires during calculations
        "model": Model, # e.g. the model that reads parameter set and returns some results
        "replica_n":3 # e.g. number of replica run for each param set
    }
}
```
`run_func` is the most important parameter that needs to be designed specifically for each problem.  It can be generally formatted as:

```py
def custom_func(paramset,args):
    return distance
```
Which receives a paramset, passed by the ABC algorithm, alongside with other parameters encapsulated as args that would be needed to calculate distance function by the user. This function ultimately returns the distance calculated for the given paramset, which is Python `float` variable.

To elaborate `run_func` for a case example:

```py
def custom_func(paramset,args):
    model = args['model']
    empirical_data = user_defined_variable
    results = model(paramset) # runs the model for the given param set
    distance = np.abs(results - empirical_data) # distance is defined in this case as absolute difference
    return distance
```
***Attentions***: `paramset` is a python dictionary contenting a set of parameter-value items as shown below. The user defined `model` must be able to integrate this parameter set. 
```py
{
    'p_name_1':2.3,
    'p_name_2':7.8
}
```
More elaboration and examples will come soon.

### Parallel or serial use

For serial use case, simply command:

```py

from ABC import tools
obj = tools.ABC(settings = settings, free_params = free_params)
obj.sample()
obj.run()
obj.postprocess()
```
To run the model in parallel using MPI, save the above mentioned commands as a script and run it from Terminal:

```py
# in terminal
mpiexec -n available_cpu_core python test.py
```
`available_cpu_core` is the CPU core number that user intend to allocate for this process. For more info, see [MPI for Python](https://mpi4py.readthedocs.io/en/stable/).

### Outputs

The posteriors are outputed for each parameter as a json file which can be found on the given output directory. A box plot is also generated to compare prior and posterior distributions in a single graph, in SVG format.

## Install/compile

###For Python development:

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

###For Cpp development:
Clone the library and add the main root as a subdirectory. Using modern Cmake, the following command will include the library:
-  `add_subdirectory("dir_to_cppyabm")`
-  `target_include_directories("your_project" "dir_to_include/ABM")`

## Authors

- Jalil Nourisa

## Useful links

 

### Acknowledgments


