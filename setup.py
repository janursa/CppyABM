import setuptools
import codecs
import os.path
here = os.path.abspath(os.path.dirname(__file__))
def read(rel_path):
    
    with codecs.open(os.path.join(here, rel_path), 'r') as fp:
        return fp.read()


def extract_longDiscription(file_name):
    with open(file_name, "r") as fh:
        long_description = fh.read()
    return long_description

import os
import pathlib

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig


class CMakeExtension(Extension):
    def __init__(self, name):
        super().__init__(name, sources=[])


class build_ext(build_ext_orig):
    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()
        build_temp = pathlib.Path(self.build_temp)
        build_temp.mkdir(parents=True, exist_ok=True)
        extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
        extdir.mkdir(parents=True, exist_ok=True)
        config = 'Debug' if self.debug else 'Release'
        cmake_args = [
            '-DBIND=TRUE',# to build for binding
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir.parent.absolute()),
            '-DCMAKE_BUILD_TYPE=' + config
        ]
        build_args = [
            '--config', config,
            '--', '-j4'
        ]
        os.chdir(str(build_temp))
        self.spawn(['cmake', str(cwd)] + cmake_args)
        if not self.dry_run:
            self.spawn(['cmake','--build', '.'] + build_args)
        os.chdir(str(cwd))

setuptools.setup(
    name="cppyabm",
    version='1.0.21',
    author="Jalil Nourisa",
    author_email="jalil.nourisa@gmail.com",
    description="General-purpose agent-based modeling framework",
    long_description=extract_longDiscription("README.md"),
    long_description_content_type="text/markdown",
    url="https://github.com/janursa/CppyABM",
    packages=['cppyabm'],
    ext_modules=[CMakeExtension('cppyabm/cppyabm')],
    cmdclass={
        'build_ext': build_ext,
    },
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.6'
)