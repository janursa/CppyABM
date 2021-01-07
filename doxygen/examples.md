@page examples
In order to demonstrate the use case of CppyABM, we design a computer model of biological tissue growth in response to an injury. The model is written in three forms of (1) purely python (2) purely Cpp, and (3) a mixed model combining both languages. Implementtion files of the examples together with Cmake files can be found on <a href="https://github.com/janursa/CppyABM/tree/master/examples">here</a>. 

## Purely Python
The documented implementation can be found on \ref tissuehealing.py. @ref purelypython
## Purely Cpp
The implementation can be found in  \ref cpp_example.h, \ref cpp_example.cpp, and \ref cpp_main.cpp. 

## Mixed CPP and Python
This example only move the Cell::step() from Cpp files to python. The Cpp implementation can be found in  \ref mixed_example.h and \ref mixed_example.cpp. Python implementation can be found on \ref mixed_script.py. In addition, we have an extra step to expose the function from cpp to python \ref mixed_binds.cpp.


