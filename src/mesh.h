
#pragma once
#include "settings.h"
using namespace std;


//!  To generate meshes
/*!
  So far, it only generates square meshes on a square domain. 
*/

class Mesh{
public:
    Mesh(int index, float x, float y):mesh_index(index), x_coord(x), y_coord(y) {

    }
    int get_meshindex(){
        return mesh_index;
    }
    vector<float> get_globalxyzcoordinates(){
        vector<float> coords = {x_coord,y_coord,0};
        return coords;
    }
    int mesh_index;
    std::vector<unsigned> neighbor_indices; //!< Indices of meshes located in the neumann boundary
    float x_coord;
    float y_coord;
    vector<shared_ptr<Mesh>>  setup_meshes(float domain_x_l, float domain_y_l, float patch_size){
        vector<shared_ptr<Mesh>> meshes;
        float mesh_size = patch_size;
        x_n() = domain_x_l/mesh_size;
        y_n() = domain_y_l/mesh_size;
        unsigned mesh_count =0;
        for (int j=0; j<y_n(); j++){ //outer vector meshes
            for (int i=0; i<x_n(); i++){ //inner vector meshe
                auto x = (mesh_size/2)+i*mesh_size;
                auto y = (mesh_size/2)+j*mesh_size;
                auto mesh_index = calculate_mesh_index(i,j);
                auto meshPtr = make_shared<Mesh>(mesh_index,x,y);
                auto neighbor_indices = find_neighborhood(i,j);
                meshPtr->neighbor_indices = neighbor_indices;
                meshes.push_back(meshPtr);
                mesh_count ++;
            }

        }
        auto message = to_string(mesh_count) + " mesh generated";
        LOG(message);
        return meshes;
    }
    // static Mesh* at(unsigned index){
    //     try {
    //         return get_modelPtr()->meshes_container[index].get();
    //     }catch(out_of_range&ofr){
    //         cerr<<"error";
    //         throw ofr;
    //         exit(2);

    //     }
    // }
    static vector<unsigned> find_neighborhood(int &x, int &y);

    
    static unsigned calculate_mesh_index(int &i, int &j){      //!< Converts x and y of a mesh to an index number
        return (j*x_n() + i);
    }

    static unsigned& x_n(){
        static unsigned var{};
        return var;
    } ; //!< Mesh number in x direction
    static unsigned& y_n(){
        static unsigned var{};
        return var;
    } ; ; //!< Mesh number in y direction
};

inline vector<unsigned> Mesh::find_neighborhood(int &x, int &y) {
    vector<unsigned> neighbor_indices;
    int neighbor_numbers = 8;
    int x_index[neighbor_numbers];
    int y_index[neighbor_numbers];
    x_index[0] = x;
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
    for (int iter = 0; iter < neighbor_numbers; iter++) {
        if (x_index[iter] < 0 or y_index[iter] < 0) continue; // for the border elements
        if (x_index[iter] >= x_n() or y_index[iter] >= y_n()) continue; // for the border elements
        int neighbor_index = calculate_mesh_index(x_index[iter],y_index[iter]);

        neighbor_indices.push_back(neighbor_index);

    }
//    for (auto index:neighbor_indices) cout<<"   "<<index<<endl;
    return neighbor_indices;
}


