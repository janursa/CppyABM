
  

## General purpose agent-based modeling library to integrate C++ and Python
CppyABM is a free open-source header-only library that enables agent-based modeling by providing essential tools in both C++ and Python as well as mixing both in a single model. The modeling in both C++ and Python follows similar semantics and style. Therefore, a model developed in one languages can be conveniently transferred to another. In addition, CppyABM provides binding tools to specifically expose certain parts of a model written in C++ for further development in Python. This enables users to take the advantage of both languages simultaneously. CppyABM uses <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">RTvisualize</a> for browser-based real-time visualization. 

The full documentation of the library is available <a href="https://janursa.github.io/CppyABM/" title="About Me">here</a>. See <a href="https://github.com/janursa/CppyABM/tree/master/examples" title="examples">examples</a> folder as showcase of CppyABM utility.


## Install/compile

### For Python development

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

### For C++ development
The library can be also installed on,
- git clone https://github.com/janursa/CppyABM 
- cd CppyABM && mkdir build && cd build
- cmake ..
- make install

Using Cmake, the library can be linked against a project,
- add_executable(your_project your_files.cpp)
- target_link_libraries(your_project  PUBLIC cppyabm::cppyabm)

The library can be also locally linked without installation by providing the `include` directory located in the root folder to the project,
- add_executable(your_project your_files.cpp )
- target_compile_features(your_project PUBLIC dir_to_include_folder)

See <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cpp" title="cpp">example</a>.

### For Python-C++ development:
In order to create a mixed model using both languages of C++ and Python, in addition to CppyABM, <a href="https://github.com/pybind/pybind11" title="pybind11">pybind11</a> needs to be installed. It can be either seperately installed according to the instruction given on <a href="https://pybind11.readthedocs.io/en/stable/installing.html" title="pybind11">pybind11-installation</a>, or can be included in a project using Cmake. The latter can be achieved by setting `PYBIND11` flag before the compilation of CppyABM. See <a href="https://github.com/janursa/CppyABM/tree/master/examples/Cppy" title="cppy">example</a>.


## Authors

- Jalil Nourisa

## Useful links
Familiarity with 
<a href="https://pybind11.readthedocs.io/en/stable/index.html" title="pybind11">pybind11</a> is helpful during the Python binding process.
 

## Contributing to CppyABM
In case of encountering an issue, pls report as an issue.


