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
# CppyABM = setuptools.Extension(
#     name='CppyABM',
#     include_dirs=[os.path.join(here, 'include'),
#     # os.path.join(here, 'build/x64-Release/_deps/pybind11-src/include/pybind11')],
#     '/Users/matin/Downloads/testProjs/CPPYABM/build/_deps/pybind11'],
#     # os.path.join(here, 'build/_deps/pybind11-src/include/pybind11')],
#     sources=[
#         os.path.join(here, 'binds', 'binds.cpp')
#     ]
# )
import os
import pathlib

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig


class CMakeExtension(Extension):

    def __init__(self, name):
        # don't invoke the original build_ext for this special extension
        super().__init__(name, sources=[])


class build_ext(build_ext_orig):

    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()
        # these dirs will be created in build_py, so if you don't have
        # any python sources to bundle, the dirs will be missing
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
        extdir.mkdir(parents=True, exist_ok=True)

        # example of cmake args
        config = 'Debug' if self.debug else 'Release'
        cmake_args = [
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir.parent.absolute()),
            '-DCMAKE_BUILD_TYPE=' + config
        ]

        # example of build args
        build_args = [
            '--config', config,
            '--', '-j4'
        ]

        os.chdir(str(build_temp))
        self.spawn(['cmake', str(cwd)] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake', '--build', '.'] + build_args)
        # Troubleshooting: if fail on line above then delete all possible 
        # temporary CMake files including "CMakeCache.txt" in top level dir.
        os.chdir(str(cwd))

setuptools.setup(
    name="cppyabm",
    version='1.0.6',
    author="Jalil Nourisa",
    author_email="jalil.nourisa@gmail.com",
    description="A general-purpose agent-based modeling framework",
    long_description=extract_longDiscription("README.md"),
    long_description_content_type="text/markdown",
    url="https://github.com/janursa/CPPYABM",
    # package_dir={'CppyABM': "build/binds"},
    # packages=['CppyABM'],
    # ext_modules=[CppyABM],
    # zip_safe=False,
    # include_package_data=True,
    # packages=setuptools.find_packages(),
    packages=['binds'],
    ext_modules=[CMakeExtension('binds/binds')],
    cmdclass={
        'build_ext': build_ext,
    },
    # install_requires=[],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6'
)