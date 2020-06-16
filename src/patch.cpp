#include <iostream>
#include "CPPYABM/bases.h"

 shared_ptr<Patch> Patch::empty_neighbor(bool quiet){
        auto neighbors = this->neighbors;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(neighbors.begin(),neighbors.end(),g);
        for (auto &neighbor:neighbors){
            if (neighbor->empty){
                return neighbor;
            }
        }
        if (!quiet) throw patch_availibility("No available patch around the agent");
        return nullptr;
}
