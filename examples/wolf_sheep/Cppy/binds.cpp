
#include <iostream>
#include "models.h"
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
    void update_repo() override {
            PYBIND11_OVERLOAD(
                void, 
                WolfSheep,      
                update_repo
                    
            );
        }
    void step() override {
            PYBIND11_OVERLOAD(
                void, 
                WolfSheep,      
                step
                    
            );
        }

};

//! Binding function. 
/*!
The name provided as module, i.e. myBinds, must be the same name given in CmakeLists.txt as argument to
pybind11_add_module. 
*/
PYBIND11_MODULE(myBinds, m) {
    using tramAnimal = bind_tools::tramAgent<WolfSheep,Animal,GrassPatch>;
    using tramGrassPatch = bind_tools::tramPatch<WolfSheep,Animal,GrassPatch>;

    auto bind_obj = bind_tools::Bind<WolfSheep,Animal,GrassPatch,tramWolfSheep,tramAnimal,tramGrassPatch>(m,"WolfSheep","Animal","GrassPatch");
    auto agent_obj = bind_obj.get_agent();
    agent_obj.def(py::init<shared_ptr<WolfSheep>,
                    string,
                    int>()
                    );
    auto patch_obj = bind_obj.get_patch();
    patch_obj.def(py::init<shared_ptr<WolfSheep>,
                    MESH_ITEM,bool,int>());

}


