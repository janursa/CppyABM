
#pragma once
#include <vector>
#include <fstream>
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
vector<MESH_ITEM> grid3(double length, double width, double height, double mesh_length, bool share);

void log_mesh(vector<MESH_ITEM> mesh, string file_name);



