CppyABM is a free open-source header-only library that enables agent-based modeling by providing essential tools in both C++ and Python as well as mixing both in a single model. The modeling in both C++ and Python follows similar semantics and style. Therefore, a model developed in one languages can be conveniently transferred to another. In addition, CppyABM provides binding tools to specifically expose certain parts of a model written in C++ for further development in Python. This enables users to take the advantage of both languages simultaneously.

# Basics of design
CppyABM provides three base classes of Env, Agent, and Patch for agent-based modeling. Users should inherit and specialize these classes based on their simulation requirements. Agent is designed to simulate moving objects; Patch is for non-moving objects; and Env stores and coordinates Agent and Patch objects. Patch object can accomodate agents and heteregenious quantities within the simulation domain. Three is a three-way connections between Env, Agent, and Patch. Env has stored pointers to Agent and Patch; Patch has pointers to the residing Agent as well as Env object; and agent has pointers to host Patch and Env object. This three-way connection enables full information retreival among the entire simulation entities. Both Agent and Patch require an input argument of Env for initialization. Also, Agent requires an identity variable termed Agent::class_name to enable multi-agent modeling; and Patch requires a MESH_ITEM object to gather three pieces of information, MESH_ITEM::index, MESH_ITEM::coords, and MESH_ITEM::neighbors_indices. There are few conventions that need further explanation to define an environment. Since Python requires object ownership, any object shared between C++ and Python ends, i.e. Patch and Agent objects, needs to be stored in Python end and referenced as a pointer in C++ end. To satisfy this requirement, two template functions of Env::generate_agent and Env::generate_patch are provided to manage the generation and storage of agents and patches. These functions need to be customized for each model (see [example](#Python+development)). A repository variable needs to be defined within Python to store created objects. Simultaneously, the objects need to be added to Env::agents and Env::patches which are the standard containers defined within C++. Using these functions, the defined agents and patches can be accessed in both C++ and Python without further measure (see [example](#Python+development)). 


# Install/compile

## - For Python development:

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

## - For Cpp development:

Clone the library and add the main root as a subdirectory. Using modern Cmake, the following command will include the library:

-  `add_subdirectory("dir_to_cppyabm")`
-  `target_include_directories("your_project" "dir_to_include/ABM")`


# C++ development
The model development in C++ is based on template programing to keep the internal connections between different class objects valid for the derived models. The primary layout of an agent-based model in C++ is,
```Cpp
#include "CppyABM/include/ABM/bases.h"
// declare the derived classes
struct myEnv;
struct myAgent;
struct myPatch;
// define myEnv
struct myEnv: public Env<myEnv,myAgent,myPatch> /*Env is inherited using template arguments*/ {
	myEnv():Env<myEnv,myAgent,myPatch>() /*a call to the base class's instructor*/{ 
	}
	shared_ptr<myAgent> generate_agent(std::string);
	shared_ptr<myPatch> generate_patch(MESH_ITEM);
};
struct myPatch: public Patch<myEnv,myAgent,myPatch> {
	myPatch(shared_ptr<myEnv> env,MESH_ITEM mesh):Patch<myEnv,myAgent,myPatch>(env,mesh)/*a call to the base class's instructor*/{

	}
};
struct myAgent: public Agent<myEnv,myAgent,myPatch> {
	myAgent(shared_ptr<myEnv> env,std::string agent_name):Agent<myEnv,myAgent,myPatch>(env,agent_name)/*a call to the base class's instructor*/{
	}
};

```
For the above layout, two generator functions are,
```cpp
shared_ptr<myAgent> myEnv::generate_agent(std::string agent_name){
	auto agent_obj = make_shared<myAgent>(this->shared_from_this(),agent_name);
	this->agents.push_back(agent_obj);
	return agent_obj;
};
shared_ptr<myPatch> myEnv::generate_patch(MESH_ITEM mesh){
	auto patch_obj = make_shared<myPatch>(this->shared_from_this(),mesh);
	this->patches.insert(pair<unsigned,shared_ptr<myPatch>>(patch_obj->index,patch_obj));
	return patch_obj;
};
```
For a complete example in C++, check out <a href="https://github.com/janursa/CppyABM/tree/master/examples/cpp" title="cpp">Cpp example</a>.

# Python development
The basic layout of an agent-based model in Python is,
```py 
from cppyabm.binds import Env, Agent, Patch

class myPatch(Patch):
	def __init__(self,env,mesh_item):
		Patch.__init__(self,env,mesh_item)

class myAgent (Agent):
	def __init__(self,env,agent_name):
		Agent.__init__(self,env = env,class_name = agent_name)

class myEnv(Env):
	def __init__(self):
		Env.__init__(self)
		self.agents_repo = []
		self.patches_repo = []
	def generate_agent(self,agent_name):
		agent_obj = myAgent(self,agent_name)
		self.agents_repo.append(agent_obj)
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self,mesh_item):
		patch_obj = myPatch(self,mesh_item)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj)
		return patch_obj
	def update_repo(self):
		for i in range(len(self.agents_repo)):
			if self.agents_repo[i].disappear==True:
				indices.append(i)
		for ele in sorted(indices, reverse = True):  
			del self.agents_repo[ele]
```
Within Python, the generator functions require an additional step of storing the created objects in the local repositories. Also, the repositories needs to be updated to remove inactive agents. For a complete example in Python, check out <a href="https://github.com/janursa/CppyABM/tree/master/examples/Py" title="py">Py example</a>.

# Binding procedure from Cpp to Python
The binding tools provided within CppyABM are based on pybind11. Generally, there are two types of binding are provisioned; exposure: to exposure certain certain functionality written in C++ code to Python; extention: to let the compiler know that the exposed functionality will be replaced within Python. The following steps are taken during the binding process:

- Expose the base functions and class types using bind_tools::expose_defaults
- Expose the derived classes using bind_tools::expose_env, bind_tools::expose_agent, and bind_tools::expose_patch. In case the model is intended for the extention binding, a trampoline needs to be designed and passed as an argument to these functions. The base trampoline classes of bind_tools::tramEnv, bind_tools::tramAgent, and bind_tools::tramPatch are provided for this purposes which need to be extended. The extention process follows the same semantics as pybin11 see <a href="https://pybind11.readthedocs.io/en/stable/advanced/classes.html" title="pybind11">pybind11</a> and <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">example</a>.
- Bind the members and methods of the derived class. In addition to the base members, the derived class can have additional members for exposure. Two functions `def` and `def_readwrite` are used for these purposes (see <a href="https://pybind11.readthedocs.io/en/stable/advanced/classes.html" title="pybind11">pybind11</a>)
- Expose Agent and Patch container using \ref EXPOSE_AGENT_CONTAINER amd \ref EXPOSE_PATCH_CONTAINER

To see an example combining all steps, see <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">binding example</a>


## Cmake
We use Cmake for model compilation and Python binding. `pybind11_add_module` provided by pybind11 takes care of a Python module creation for the given set of C++ files. See <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">binding example</a> for an example.

# Visualization
We use <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">RTvisualize</a> for real-time visualization of the agent-based modeling. An example can be found on <a href="https://github.com/janursa/CppyABM/tree/master/examples/Py" title="cppy">monitor_script</a>.

# Examples
In order to demonstrate the use case of CppyABM, we design a computer model of the biological tissue growth in response to injury. For description of the problem, see the article. Implementtion files of the examples together with Cmake files can be found on <a href="https://github.com/janursa/CppyABM/tree/master/examples">here</a>. 

