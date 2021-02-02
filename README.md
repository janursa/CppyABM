
  

# General purpose agent-based modeling library to integrate C++ and Python
CppyABM allows agent-based modeling by providing essential tools in both C++ and Python as well as mixing both in a single development. The modeling in both C++ and Python follows similar semantics and style. Therefore, a model developed in one languages can be conveniently transferred to another. In addition, the provided binding tools in CppyABM enables to specifically expose certain parts of a model written in C++ for further development in Python. This enables users to take the advantage of both languages simultaneously. CppyABM uses <a href="https://github.com/janursa/RTvisualize" title="RTvisualize">here</a> for browser-based real-time visualization. 

The full documentation of the library is available <a href="https://janursa.github.io/CppyABM/" title="About Me">here</a>. See examples (link) folder as showcase of CppyABM utility.


##Install/compile

###For Python development:

Using pip manager:

-  `pip install --upgrade cppyabm`

Or, download the package and in the root folder, command:

-  `python3 setup.py install`

###For Cpp development:
Clone the library and add the main root as a subdirectory. Using modern Cmake, the following command will include the library:
-  `add_subdirectory("dir_to_cppyabm")`
-  `target_include_directories("your_project" "dir_to_include/ABM")`
 

## Authors

- Jalil Nourisa

## Useful links

 

### Contributing to CppyABM
In case of encountering an issue, pls report as an issue.


