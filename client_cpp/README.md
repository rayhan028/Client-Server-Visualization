# C++ TCP Client

## Build
mkdir build
cd build
cmake ..
make


## Run
/client

Make sure the Python server is running first.

cd server_python
python3 server.py

cd client_cpp
mkdir build && cd build
cmake ..
make
./client

Try commands:

PING

TIME

EXIT
