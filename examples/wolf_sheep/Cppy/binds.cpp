
#include <iostream>
#include "models.h"
// #include <pybind11/stl.h>
// #include <pybind11/stl_bind.h>
// #include <pybind11/complex.h>
// #include <pybind11/functional.h>
#include "cppyabm/bind_tools.h"


EXPOSE_AGENT_CONTAINER(Animal);
EXPOSE_PATCH_CONTAINER(GrassPatch);

struct tramWolfSheep : public WolfSheep  {
    using WolfSheep::WolfSheep;

    shared_ptr<Animal> generate_agent(string agent_name,int wolf_energy, int sheep_energy) override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Animal>, 
            WolfSheep,      
            generate_agent,
            agent_name, wolf_energy,sheep_energy
        );
    }
    shared_ptr<GrassPatch> generate_patch(MESH_ITEM mesh) override {
        PYBIND11_OVERLOAD(
            shared_ptr<GrassPatch>, 
            WolfSheep,      
            generate_patch,
            mesh
        );
    }

};
struct tramAnimal : public Animal  {
    using Animal::Animal;
};
struct tramGrassPatch : public GrassPatch  {
    using GrassPatch::GrassPatch;

};

//! Binding function. 
/*!
The name provided as module, i.e. myBinds, must be the same name given in CmakeLists.txt as argument to
pybind11_add_module. 
*/
PYBIND11_MODULE(myBinds, m) {

 
    // 
    // auto bind_obj = bind_tools::Bind<myEnv,Animal,GrassPatch,tramMyEnv,tramAnimal,tramGrassPatch>(m,"myEnv","Animal","GrassPatch");
    // auto patch_obj = bind_obj.get_patch();
    // patch_obj.def(py::init<shared_ptr<myEnv>,
    //                 MESH_ITEM,
    //                 bool,
    //                 int>()
    //                 );
    // 
    // agent_obj.def("step",&Animal::step);
    // bind_tools::expose_env<myEnv,Animal,GrassPatch,tramMyEnv>(m,"myEnv");
    // bind_tools::expose_agent<myEnv,Animal,GrassPatch,tramAnimal>(m,"Animal");
    // bind_tools::expose_patch<myEnv,Animal,GrassPatch,tramGrassPatch>(m,"GrassPatch");
    // bind_tools::expose_env<myEnv,Animal,GrassPatch,tramMyEnv>(m,"myEnv");
    // auto agent_obj = bind_obj.get_agent();
    // agent_obj.def_readwrite("cycle_t",&Cell::cycle_t);
    // 
    auto bind_obj = bind_tools::Bind<WolfSheep,Animal,GrassPatch,tramWolfSheep,tramAnimal,tramGrassPatch>(m,"WolfSheep","Animal","GrassPatch");
    // auto env_obj = bind_obj.get_env();
    // env_obj.def("setup_animals",&WolfSheep::setup_animals);
    auto agent_obj = bind_obj.get_agent();
    agent_obj.def(py::init<shared_ptr<WolfSheep>,
                    string,
                    int>()
                    );
    auto patch_obj = bind_obj.get_patch();
    patch_obj.def(py::init<shared_ptr<WolfSheep>,
                    MESH_ITEM,bool,int>());
    // env_obj.def("setup_animals",&WolfSheep::setup_animals);
    // bind_tools::expose_agent<WolfSheep,Animal,GrassPatch,tramAnimal>(m,"Animal");
    // bind_tools::expose_patch<WolfSheep,Animal,GrassPatch,tramGrassPatch>(m,"GrassPatch");
    // bind_tools::expose_env<WolfSheep,Animal,GrassPatch,tramWolfSheep>(m,"WolfSheep");

}


