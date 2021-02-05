FROM python:3.7-slim

LABEL description="Container for CppyABM" 

# set a directory for the app
WORKDIR /usr/src/app
# install 
RUN apt-get update && apt-get install -y --no-install-recommends g++ python3-dev make cmake git 
&& pip3 install pytest && git clone https://github.com/pybind/pybind11.git && cd pybind11 && mkdir build && cd build && cmake .. && make install
#&& git clone git clone https://github.com/janursa/CppyABM  && mkdir build && cd build 
#apt-get install -y g++ make cmake install git 
#&& apt-get install -y python3-dev libssl-dev

# copy all the files to the container
COPY . .

# install dependencies
# RUN pip install --no-cache-dir -r requirements.txt

EXPOSE 8050
# run the command
# CMD ["python", "./script.py"]
