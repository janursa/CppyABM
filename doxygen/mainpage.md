## Inheritence-based modeling
CppyABM provides three base classes of Env, Agent, and Patch for agent-based modeling. Users should inherit and specialize these classes based on their simulation requirements. Agent is designed to simulate moving objects; Patch is for non-moving objects; and Env stores and coordinates Agent and Patch objects. Patch object can accomodate agents and heteregenious quantities within the simulation domain. Three is a three-way connections between Env, Agent, and Patch. Env has stored pointers to Agent and Patch; Patch has pointers to the residing Agent as well as Env object; and agent has pointers to host Patch and Env object. This three-way connection enables full information retreival among the entire simulation entities.

Agent and Patch require an input argument of Env for initialization. Also, Agent requires an identity variable termed \ref Agent::class_name to enable multi-agent modeling. There are few conventions that need further explanation to define an environment. Since Python requires object ownership, any object shared between C++ and Python ends, i.e. Patch and Agent objects, needs to be stored in Python end and referenced as a pointer in C++ end. To satisfy this requirement, two template functions of \ref Env::generate_agent and \ref Env::generate_patch are provided to manage the generation and storage of agents and patches. These functions need to be customized for each model. A repository variable needs to be defined in Python end to store created objects. Simultaneously, the objects need to be added to Env::agents and Env::patches which are the standard containers. The examples is given in \ref Domain. Using these functions, the defined agents and patches can be accessed in both C++ and Python without further measure.

# Binding procedure from Cpp to Python
lets bind it [here](@ref binding).

# Usefull build-in functions

# Examples
Find the examples [here](@ref examples)
