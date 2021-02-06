FROM ubuntu

LABEL description="CppyABM: a container for C++ development" 

# set a directory for the app
WORKDIR /usr/src/app
# copy all the files to the container
COPY . .
# install 
RUN apt-get update && apt-get install -y --no-install-recommends g++ make cmake &&  apt-get install -y git && git clone https://github.com/janursa/CppyABM && cd CppyABM && mkdir build && cd build && cmake .. && make install


#&& apt-get install -y python3-dev libssl-dev



# install dependencies
# RUN pip install --no-cache-dir -r requirements.txt

# EXPOSE 8050
# run the command
# CMD ["python", "./script.py"]
