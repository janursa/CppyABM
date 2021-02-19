
## General purpose agent-based modeling library to integrate C++ and Python
CppyABM is a free open-source header-only library that enables agent-based modeling by providing essential tools in both C++ and Python as well as mixing both in a single model. The modeling in both C++ and Python follows similar semantics and style. Therefore, a model developed in one languages can be conveniently transferred to another. In addition, CppyABM provides binding tools to specifically expose certain parts of a model written in C++ for further development in Python. This enables users to take the advantage of both languages simultaneously. CppyABM uses <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">RTvisualize</a> for browser-based real-time visualization. 

The documentation of the library is available <a href="https://janursa.github.io/CppyABM/" title="About Me">here</a>. See <a href="https://github.com/janursa/CppyABM/tree/master/examples" title="examples">examples</a> folder as showcase of CppyABM utility.


## Install
For an extended version of the available installation options, see <a href="https://janursa.github.io/CppyABM/" title="About Me">documentation</a>.

### For Python development

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

### For C++ development
The library can be also installed solely for C++ development using,
- git clone https://github.com/janursa/CppyABM 
- cd CppyABM && mkdir build && cd build
- cmake .. -DCPP=TRUE
- make install


### For Python-C++ development
In order to create a mixed model using both languages of C++ and Python, in addition to CppyABM, <a href="https://github.com/pybind/pybind11" title="pybind11">pybind11</a> needs to be installed. It can be either seperately installed according to the instruction given on <a href="https://pybind11.readthedocs.io/en/stable/installing.html" title="pybind11">pybind11-installation</a>, or can be included in a project using Cmake. See  <a href="https://janursa.github.io/CppyABM/" title="About Me">documentation</a>.

## Docker
The following docker image will provide an isolated environment where the latest version of CppyABM and pybind11 are insalled.

$ docker run --rm -t -i janursa/cppyabm /bin/bash


## License
This project is licensed under the MIT License - see the LICENSE.md file for details

## To cite
Cite this library using [![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4541247.svg)](https://doi.org/10.5281/zenodo.4541247).


## Authors

- Jalil Nourisa

## Useful links
Familiarity with 
<a href="https://pybind11.readthedocs.io/en/stable/index.html" title="pybind11">pybind11</a> is helpful during the Python binding process.
 

## Contributing to CppyABM
In case of encountering a problem, pls report it as an issue or contant the author (jalil.nourisa@gmail.com)


