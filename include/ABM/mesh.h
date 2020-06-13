
#pragma once
#include <vector>
#include "common.h"
using namespace std;


//!  To generate meshes
/*!
  So far, it only generates square meshes on a square domain. 
*/
struct MESH_ITEM {
    unsigned index;
    vector<double> coords;
    std::vector<unsigned> neighbors_indices;
}; //!< Mesh item
struct mesh_tools{ 

    static vector<MESH_ITEM> grid(double x, double y, double size){
        auto calculate_mesh_index = [](unsigned i, unsigned j,unsigned x_n) {
            return (j*x_n + i);
        };
        auto find_neighborhood=[&](unsigned x, unsigned y,unsigned x_n, unsigned y_n){
            vector<unsigned> neighbor_indices;
            unsigned neighbor_numbers = 8;
            unsigned x_index[neighbor_numbers];
            unsigned y_index[neighbor_numbers];
            x_index[0] = x; // TODO: do it more smartly
            y_index[0] = y + 1;

            x_index[1] = x;
            y_index[1] = y - 1;

            x_index[2] = x + 1;
            y_index[2] = y;

            x_index[3] = x - 1;
            y_index[3] = y;

            x_index[4] = x + 1;
            y_index[4] = y + 1;

            x_index[5] = x + 1;
            y_index[5] = y - 1;

            x_index[6] = x - 1;
            y_index[6] = y + 1;

            x_index[7] = x - 1;
            y_index[7] = y - 1;
            for (unsigned iter = 0; iter < neighbor_numbers; iter++) {
                if (x_index[iter] < 0 or y_index[iter] < 0) continue; // for the border elements
                if (x_index[iter] >= x_n or y_index[iter] >= y_n) continue; // for the border elements
                auto neighbor_index = calculate_mesh_index(x_index[iter],y_index[iter],x_n);

                neighbor_indices.push_back(neighbor_index);

            }
            return neighbor_indices;
        };
        
        vector<MESH_ITEM> meshes;
        
            
        unsigned x_n = x/size;
        unsigned y_n = y/size;
        double mesh_size = size;
        unsigned mesh_count =0;
        // ** some small funcs **//
        

        for (unsigned j=0; j<y_n; j++){ //outer vector meshes
            for (unsigned i=0; i<x_n; i++){ //inner vector meshe
                auto x = (mesh_size/2)+i*mesh_size;
                auto y = (mesh_size/2)+j*mesh_size;
                double z = 0;
                vector<double> coords = {x,y,z};
                auto mesh_index = calculate_mesh_index(i,j,x_n);
                auto neighbor_indices = find_neighborhood(i,j,x_n,y_n);
                MESH_ITEM mesh_item = {mesh_index,coords,neighbor_indices};
                meshes.push_back(mesh_item);
                mesh_count ++;
            }

        }
        auto message = to_string(mesh_count) + " mesh generated";
        LOG(message);
        return meshes;
    };
};  // namespace meshes

// inline void mesh_tools::grid::print_to_file(string file_name){
//     auto file_name_mod = main_output_folder+"/"+file_name;
//     ofstream ID(file_name_mod);          
//     for (auto &item:this->meshes){
//         ID<<item.index<<"\t";

//         ID<<item.coords.x<<" "<<item.coords.y<<" "<<item.coords.z<<"\t";
//         for (auto&el:item.neighbors_indices){
//             ID<<el<<" ";
//         }

//         ID<<endl;
//     }
//     ID.close();
// }



