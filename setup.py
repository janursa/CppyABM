import setuptools
import codecs
import os.path
here = os.path.abspath(os.path.dirname(__file__))
def read(rel_path):
    
    with codecs.open(os.path.join(here, rel_path), 'r') as fp:
        return fp.read()

# def get_version(rel_path):
#     for line in read(rel_path).splitlines():
#         if line.startswith('__version__'):
#             delim = '"' if '"' in line else "'"
#             return line.split(delim)[1]
#     else:
#         raise RuntimeError("Unable to find version string.")

def extract_longDiscription(file_name):
    with open(file_name, "r") as fh:
        long_description = fh.read()
    return long_description
CppyABM = setuptools.Extension(
    name='CppyABM',
    include_dirs=[os.path.join(here, 'include'),
    os.path.join(here, 'build/x64-Release/_deps/pybind11-src/include/pybind11')],
    sources=[
        os.path.join(here, 'binds', 'binds.cpp')
    ]
)
setuptools.setup(
    name="CppyABM",
    version='1.0.2',
    author="Jalil Nourisa",
    author_email="jalil.nourisa@gmail.com",
    description="A general-purpose agent-based modeling framework",
    long_description=extract_longDiscription("README.md"),
    long_description_content_type="text/markdown",
    url="https://github.com/janursa/CPPYABM",
    # package_dir={'CppyABM': "build/x64-Release/binds"},
    # packages=['CppyABM'],
    ext_modules=[CppyABM],
    zip_safe=False,
    include_package_data=True,
    # packages=setuptools.find_packages(),
    install_requires=[],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6',
)