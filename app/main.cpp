#ifdef DEALII
#include <deal.II/grid/tria.h>
#include <deal.II/grid/tria_accessor.h>
#include <deal.II/grid/tria_iterator.h>
#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_out.h>
#include <deal.II/grid/grid_in.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace dealii;
using namespace std;

void _dealii() {
    auto first_grid = [&]() {
        Triangulation<2> tria;
        GridGenerator::hyper_cube(tria);
        tria.refine_global(4); // a cube with an area and patch size 
        for (auto& cell : tria.active_cell_iterators()) {
            cout << cell->index() << endl;
        }
        map<unsigned, double> inputs;

        map<unsigned, double> outputs;

    };

    auto second_grid = [&]() {
        Triangulation<2> tria;
        ifstream file("grid-1.vtk");
        GridIn<2, 2> grid;
        grid.attach_triangulation(tria);
        grid.read_vtk(file);
        for (auto& cell : tria.active_cell_iterators()) {
            cout << cell->index() << endl;
        }
        cout << tria.n_active_cells() << endl;
    };
    first_grid();
    //second_grid();
}


#endif //dealii
#ifdef VTK
#include <vtkGenericDataObjectReader.h>
#include <vtkStructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <string>
void _vtk()
{
    cout << "insde vtk" << endl;
    // simply set filename here (oh static joy)
    std::string inputFilename = "setYourPathToVtkFileHere";

    // Get all data from the file
    vtkSmartPointer<vtkGenericDataObjectReader> reader =
        vtkSmartPointer<vtkGenericDataObjectReader>::New();
    reader->SetFileName(inputFilename.c_str());
    reader->Update();

    // All of the standard data types can be checked and obtained like this:
    if (reader->IsFilePolyData())
    {
        std::cout << "output is a polydata" << std::endl;
        vtkPolyData* output = reader->GetPolyDataOutput();
        std::cout << "output has " << output->GetNumberOfPoints() << " points." << std::endl;
    }

    
}
#endif //vtk


int main()
{
#ifdef DEALII
    _dealii();
#endif //DEALII
#ifdef VTK
    _vtk();
#endif //vtk


}


