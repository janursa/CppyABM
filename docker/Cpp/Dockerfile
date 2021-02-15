FROM ubuntu

LABEL description="CppyABM: a container for C++ development" 

# set a directory for the app
WORKDIR /dir
# copy all the files to the container
COPY . .

# install 
RUN apt-get update && apt-get install -y --no-install-recommends g++ make cmake &&  apt-get install -y git && git clone https://github.com/janursa/CppyABM && cd CppyABM && mkdir build && cd build && cmake .. -DCPP=TRUE && make install && cp -r ../examples/Cpp/ ../../ 



EXPOSE 9880
# run the command
# CMD ["g++", "cpp_main.cpp","-o","appABM"]
