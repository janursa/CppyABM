FROM python:3.7-slim

LABEL description="CppyABM: a container for C++ and Python development" 

# set a directory for the app
WORKDIR /dir
# copy all the files to the container
# COPY . .

# install 
RUN apt-get update && apt-get install -y --no-install-recommends make cmake &&  apt-get install -y git python3-dev python3-pip && pip install "pybind11[global]" && pip install numpy pandas && git clone https://github.com/janursa/CppyABM && cd CppyABM && python3 setup.py install&& cd build && cmake .. -DCPP=TRUE && make install && pip install RTvisualize 



EXPOSE 8050
# run the command
#CMD ["g++", "cpp_main.cpp","-o","appABM"]
