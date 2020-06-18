#include "CPPYABM/mesh.h"
vector<MESH_ITEM> grid(double length, double width, double mesh_length, bool share){
        auto calculate_mesh_index = [](unsigned i, unsigned j,unsigned x_n) {
            return (j*x_n + i);
        };
        auto find_neighborhood=[&](unsigned i, unsigned j,unsigned x_n, unsigned y_n){
            vector<unsigned> neighbor_indices;
            unsigned neighbor_numbers = 8;
            int x_index[neighbor_numbers];
            int y_index[neighbor_numbers];
            int counter = 0;
            // find the neighbors
            for (int ii = -1; ii <= 1; ii++){
                for (int jj = -1; jj <= 1; jj++ ){
                    if (ii == 0 and jj == 0) continue;
                    x_index[counter] = i + ii; 
                    y_index[counter] = j + jj;
                    counter ++;
                }
            }
            
            for (unsigned iter = 0; iter < neighbor_numbers; iter++) {
                // correct for the border elements
                if (x_index[iter] < 0){
                    if (share){
                        x_index[iter] = x_n - 1;
                    } else continue;
                }
                if (y_index[iter] < 0){
                    if (share) {
                        y_index[iter] = y_n -1;
                    } else continue;
                } 
                if (x_index[iter] >= x_n){
                    if (share){
                        x_index[iter] = 0;
                    } else continue;
                } 
                if (y_index[iter] >= y_n){
                    if (share){
                        y_index[iter] = 0;
                    } else continue;
                } 
                auto neighbor_index = calculate_mesh_index(x_index[iter],y_index[iter],x_n);

                neighbor_indices.push_back(neighbor_index);
                
            }
            return neighbor_indices;
        };
        
        vector<MESH_ITEM> meshes;
        
            
        unsigned x_n = length/mesh_length;
        unsigned y_n = width/mesh_length;
        unsigned mesh_count =0;
        // ** some small funcs **//
        

        for (unsigned j=0; j<y_n; j++){ //outer vector meshes
            for (unsigned i=0; i<x_n; i++){ //inner vector meshe
                auto x = (mesh_length/2)+i*mesh_length;
                auto y = (mesh_length/2)+j*mesh_length;
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