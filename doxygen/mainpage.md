

CppyABM is a free open-source header-only library that enables agent-based modeling both C++ and Python.  The model development follows similar semantics and style in both languages. Therefore, a model developed in one languages can be conveniently transferred to another. In addition, CppyABM provides essential binding tools to specifically expose certain parts of a model written in C++ for further development in Python. This enables users to take the advantage of both languages simultaneously.

## Basics of design
CppyABM provides three base classes of Env, Agent, and Patch for agent-based modeling. These classes can be inherited and extended based on specific simulation requirements. Agent is designed to simulate moving objects; Patch is for non-moving objects; and Env stores and coordinates Agent and Patch objects. Patch object can accomodate agents and heteregenious quantities within the simulation domain. There is a three-way connections between Env, Agent, and Patch. Env has stored pointers to Agent and Patch; Patch has pointers to the residing Agent as well as Env object; and agent has pointers to host Patch and Env object. This three-way connection enables full information retreival among the entire simulation entities. Both Agent and Patch require an input argument of Env for initialization. Also, Agent requires an identity variable termed Agent::class_name to enable multi-agent modeling; and Patch requires a MESH_ITEM object to gather three pieces of information, MESH_ITEM::index, MESH_ITEM::coords, and MESH_ITEM::neighbors_indices. There are few conventions that need further explanation to define an environment. Since Python requires object ownership, any object shared between C++ and Python ends, i.e. Patch and Agent objects, needs to be stored in Python end and referenced as pointers in C++ end. To satisfy this requirement, two template functions of Env::generate_agent and Env::generate_patch are provided to manage the generation and storage of agents and patches. These functions need to be customized for each model (see [example](#python-development)). A repository variable needs to be defined within Python to store created objects. Simultaneously, the objects need to be added to Env::agents and Env::patches which are the standard containers defined within C++. Using these functions, the defined agents and patches can be accessed in both C++ and Python without further measure (see [example](#python-development)). 

## Install {#install}
### For Python development

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

### For C++ development
The library can be also installed solely for C++ development using,
- git clone https://github.com/janursa/CppyABM 
- cd CppyABM && mkdir build && cd build
- cmake .. -DCPP=TRUE
- make install
#### Cmake

Once installed, the library can be linked using Cmake as,
```c
add_executable(your_project your_files.cpp)
target_link_libraries(your_project  PUBLIC cppyabm::cppyabm)
```
The library can be also locally linked without installation by providing the `include` directory located in the root folder to the project,
```py
add_executable(your_project your_files.cpp )
target_compile_features(your_project PUBLIC dir_to_include_folder)
```
To automatically clone CppyABM and link against the project,
```py
SET(CPP TRUE)
include(FetchContent)
FetchContent_Declare(
	  cppyabm
	  GIT_REPOSITORY https://github.com/janursa/CppyABM.git
	  GIT_TAG        master
	)
FetchContent_GetProperties(cppyabm)
if(NOT cppyabm_POPULATED)
	FetchContent_Populate(cppyabm)
	add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})
endif()
target_include_directories(project_name PUBLIC ${cppyabm_SOURCE_DIR}/include)
```

For a showcase, see <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cpp" title="cpp">example</a>.

### For Python-C++ development:
In order to create a mixed model using both languages of C++ and Python, in addition to CppyABM, <a href="https://github.com/pybind/pybind11" title="pybind11">pybind11</a> needs to be installed. It can be either seperately installed according to the instruction given on <a href="https://pybind11.readthedocs.io/en/stable/installing.html" title="pybind11">pybind11-installation</a>, or can be included in a project using Cmake. 
#### Cmake {#cmake}
The latter can be also achieved by using,
```py
include(FetchContent)
FetchContent_Declare(
	  cppyabm
	  GIT_REPOSITORY https://github.com/janursa/CppyABM.git
	  GIT_TAG        master
	)
FetchContent_GetProperties(cppyabm)
if(NOT cppyabm_POPULATED)
	FetchContent_Populate(cppyabm)
	add_subdirectory(${cppyabm_SOURCE_DIR} ${cppyabm_BINARY_DIR})
  endif()
target_include_directories(project_name PUBLIC ${cppyabm_SOURCE_DIR}/include)
```
For a showcase, see <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">example</a>.

## C++ development {#cpp-development}
The model development in C++ is based on template programing to keep the internal connections between different class objects valid for the derived models. The primary layout of an agent-based model in C++ is,
```Cpp
#include "cppyabm/bases.h"
// declare the derived classes
struct myEnv;
struct myAgent;
struct myPatch;
// define myEnv
struct myEnv: public Env<myEnv,myAgent,myPatch> /*Env is inherited using template arguments*/ {
	// just to assure that the base constructor is implemented
	using baseEnv = Env<myEnv,myAgent,myPatch>;
	using baseEnv::baseEnv;
	myEnv():baseEnv() /*a call to the base class's instructor*/{ 
	}
	shared_ptr<myAgent> generate_agent(std::string);
	shared_ptr<myPatch> generate_patch(MESH_ITEM);
};
struct myPatch: public Patch<myEnv,myAgent,myPatch> {
	using basePatch = Patch<myEnv,myAgent,myPatch>;
	basePatch::basePatch;
	// any Patch derived class should receive these arguments
	myPatch(shared_ptr<myEnv> env,MESH_ITEM mesh):basePatch(env,mesh)/*a call to the base class's instructor*/{

	}
};
struct myAgent: public Agent<myEnv,myAgent,myPatch> {
	using baseAgent = Agent<myEnv,myAgent,myPatch>;
	using baseAgent::baseAgent;
	// any derived agent should receive these arguments
	myAgent(shared_ptr<myEnv> env,std::string agent_name):baseAgent(env,agent_name)/*a call to the base class's instructor*/{
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
For a complete example in C++, check out <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cpp" title="cpp">Cpp example</a>.

## Python development {#python-development}
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
		self.agents_repo = [] # local repository
		self.patches_repo = [] # local repository
	def generate_agent(self,agent_name):
		agent_obj = myAgent(self,agent_name)
		self.agents_repo.append(agent_obj) # extra step
		self.agents.append(agent_obj)
		return agent_obj
	def generate_patch(self,mesh_item):
		patch_obj = myPatch(self,mesh_item)
		self.patches.append(patch_obj);
		self.patches_repo.append(patch_obj) # extra step
		return patch_obj
	def update_repo(self): # to update the repository if an agent is inactive/dead
		for i in range(len(self.agents_repo)):
			if self.agents_repo[i].disappear==True:
				indices.append(i)
		for ele in sorted(indices, reverse = True):  
			del self.agents_repo[ele]
```
Within Python, the generator functions require an additional step of storing the created objects in the local repositories. Also, the repositories needs to be updated to remove inactive agents. For a complete example in Python, check out <a href="https://github.com/janursa/CppyABM/tree/master/examples/Py" title="py">Py example</a>.

## Mixed Python and C++ development
A model writen in C++ can be further extended within Python by the creation of bindings. Generally, there are two types of binding approach; embeding: to expose certain functionality already written in C++ code to Python; extending: to bind a functionality that needs to be (further) implemented in Python. We currently use pybind11 to generate Python bindings. By having the derived classes of `myEnv`, `myAgent`, and `myPatch`, the binding template looks like,
```Cpp
#include "cppyabm/bind_tools.h"
using namespace bind_tools;

PYBIND11_MODULE(module_name, m) {
    auto bind_obj = Bind<myEnv,myAgent,myPatch,tramEnv,tramAgent,tramPatch>(m,"myEnv","myAgent","myPatch");
}
```
where `module_name` is the Python module created for binding. The template class `bind_tools::Bind` is the main binding tool. It receives the derived classes as template arguments together with the extension schemes (prefixed `tram`). It also receives `m`together with the binding names used for the derived classes (`"myEnv","myAgent","myPatch"`). `m` is a `py::module` object created automatically by pybind11. The exposure schemes are  <a href="https://pybind11.readthedocs.io/en/stable/advanced/classes.html" title="py">trampoline</a> classes introduced in pybind11. CppyABM provides three trampoline  classes of `tramEnv`,`tramAgent`,`tramPatch` that can be inherited in order to declare any extention to the compiler (see [Extention](#extention)). For the exposure purposes, these classes can be directly passed to the `bind_tools::Bind` without modification,
```Cpp
#include "cppyabm/bind_tools.h"
using namespace bind_tools;
PYBIND11_MODULE(module_name, m) {
	using tramEnv = tramEnv<myEnv,myAgent,myPatch>; // just use the base class
	using tramAgent = tramAgent<myEnv,myAgent,myPatch>; // just use the base class
	using tramPatch = tramPatch<myEnv,myAgent,myPatch>; // just use the base class
	auto bind_obj = Bind<myEnv,myAgent,myPatch,tramEnv,tramAgent,tramPatch>(m,"myEnv","myAgent","myPatch");
}
```
The `bind_tools::Bind`  automatically binds the class members and functions of the base classes, i.e. `Env`,`Agent`, and `Patch`, as the members of the derived classes. However, the specific attributes of the derived class needs to be bind in a seperate step. For that, `bind_tools::Bind` provides three functions of `get_env`, `get_agent` and `get_patch` to have access to the exposed classes in order to attach further class members.  Two pybind11 functions of `def` and `def_readwrite` can be used to bind class functions and members, respectively. For example, assuming that `myEnv` has a function named `myEnvFunc`, and `myAgent` has a member `myAgentMem`, the following code will expose these class attibutes,
```Cpp
	auto bind_obj = Bind<myEnv,myAgent,myPatch,tramEnv,tramAgent,tramPatch>(m,"myEnv","myAgent","myPatch");
	auto p_env_obj = bind_obj.get_env();// returns the bind env object to attach class members and functions
	p_env_obj.def("myEnvFunc",&myEnv::myEnvFunc); // attach the function
	auto p_agent_obj = bind_obj.get_agent(); // returns the bind agent 
	p_agent_obj.def_readwrite("myAgentMem",&myAgent::myAgentMem); // attach the class member
```
#### Extention {#extention}
For the extention purposes, the base classes of bind_tools::tramEnv, bind_tools::tramAgent, bind_tools::tramPatch can be inherited and extended prior to a call to `bind_tools::Bind`. We use pybind11 semantics for the creation of <a href="https://pybind11.readthedocs.io/en/stable/advanced/classes.html" title="py">trampoline</a>. Assuming that the class function of `myEnvFunc` belonging to `myEnv` needs to be overwritten within Python, the following trampoline can be defined,
```Cpp
#include "cppyabm/bind_tools.h"
using tramEnv = bind_tools::tramEnv<myEnv,myAgent,myPatch>;  // just an alias
struct tramMyEnv : public  tramEnv{ //inherit from the base class
    using tramEnv::tramEnv; // using constructor of the parent class
    void myEnvFunc(int input_arg) override { // declare this function for overloading within Python
        PYBIND11_OVERLOAD(
            void, // output type
            myEnv, // which class it belogs to    
            myEnvFunc, // under what name it will be called within Python
            input_arg //input name
        );
    }
};
```
Once the specialized trampoline is defined, 
```Cpp
	auto bind_obj = Bind<myEnv,myAgent,myPatch,tramEnv/*the specialized trampoline**/,tramMyAgent,tramPatch>(m,"myEnv","myAgent","myPatch");
```
#### Opaque exposure
By default, pybind11 converts the content of the variables based on STL containers. This prevents pass-by-reference and therefore any change made to the variable within Python is not reflected within C++. To prevent this, the STL-based variables needs to be exposed opaque. Within CppyABM, two variables of `agents` and `patches` as the default containers are based on STL. Therefore, prior to binding process, the following functions need to be executed,
```Cpp
EXPOSE_PATCH_CONTAINER(myPatch); // receives the derived Patch class
EXPOSE_AGENT_CONTAINER(myAgent); // receives the derived Agent class
PYBIND11_MODULE(module_name, m)  {
	...
}
```


To see an example combining all steps, see <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">binding example</a>


#### Cmake
We use Cmake for the model compilation and generation of Python binding. <a href="https://pybind11.readthedocs.io/en/stable/compiling.html" title="pybind11">`pybind11_add_module`</a> provided by pybind11 takes care of a Python module creation for the given set of C++ files. See <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">binding example</a> for an example.

## Visualization
We use <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">RTvisualize</a> for real-time visualization of the agent-based modeling. The realization of agents in 2D and 3D simulations can be done by using Scatter plot 2D and Scatter plot 3D (see <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">here</a>). The heteregenous distribution of Patch quantities within the simulation domain can be monitored used Map plot. And the evolution of a variable in the course of simulation can be monitored using Line plots. An example can be found on <a href="https://github.com/janursa/CppyABM/tree/master/examples/Py" title="cppy">here</a>.

## Examples
In order to demonstrate the use case of CppyABM, we design a computer model of the biological tissue growth in response to injury. For description of the problem, see the article. Implementtion files of the examples together with Cmake files can be found on <a href="https://github.com/janursa/CppyABM/tree/master/examples">here</a>. 

## To cite
Cite this library using [![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4541247.svg)](https://doi.org/10.5281/zenodo.4541247).

## License
This project is licensed under the MIT License - see the LICENSE.md file for details

## Authors

- Jalil Nourisa

## Useful links
Familiarity with 
<a href="https://pybind11.readthedocs.io/en/stable/index.html" title="pybind11">pybind11</a> is helpful during the Python binding process.
 
## Contributing to CppyABM
In case of encountering a problem, pls report it as an issue or contant the author (jalil.nourisa@gmail.com)

