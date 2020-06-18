
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


vector<MESH_ITEM> grid(double length, double width, double mesh_length, bool share = false);


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



