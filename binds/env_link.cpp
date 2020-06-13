#include "env.cpp"
struct PyEnv : public Env {
    using Env::Env;
    void setup() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            setup         
        );
    }
    shared_ptr<Patch> generate_patch() override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Patch>, 
            Env,      
            generate_patch         
        );
    }
    shared_ptr<Agent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Agent>, 
            Env,      
            generate_agent,
            agent_name         
        );
    }
    void update_repo() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            update_repo
                
        );
    }
};
void link_env(py::module m){
    py::class_<Env,PyEnv,std::shared_ptr<Env>>(m,"Env",py::dynamic_attr())
        .def(py::init<>())
        .def("check",&Env::check)
        .def("place_agent_randomly",&Env::place_agent_randomly)
        .def("setup_domain",&Env::setup_domain)
        .def("step_agents",&Env::step_agents)
        .def("step_patches",&Env::step_patches)
        .def("place_agent",&Env::place_agent)
        .def("update",&Env::update)
        .def("setup_agents",&Env::setup_agents)
        .def("count_agents",&Env::count_agents)
        .def("collect_from_patches",&Env::collect_from_patches)
        .def_readwrite("patches",&Env::patches)
        .def_readwrite("agents",&Env::agents);
}