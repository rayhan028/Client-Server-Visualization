# Simple Client–Server Application  
### Python TCP Server + C++ TCP Client

This project demonstrates a minimal, educational client–server system implemented using two different programming languages:

- **Python** for the server  
- **C++** for the client  

The goal is to provide a clean, easy-to-understand example of TCP socket communication across languages.

---

## Features

- **TCP communication** between Python and C++  
- **Threaded Python server** that handles multiple clients  
- **Interactive C++ client**  
- **Simple command protocol**:
  - `PING` → responds with `PONG`  
  - `TIME` → returns the current server time  
  - `EXIT` → closes the connection  
- **Beginner-friendly and extensible design**

---

## How It Works

1. The Python server listens on `127.0.0.1:5000`.  
2. Each connected client is handled in a dedicated thread.  
3. The C++ client connects and allows users to send simple text commands.  
4. The server returns appropriate responses for each command.

---

## Running the Application

### 1. Start the Python Server
```bash
cd server_python
python3 server.py

## Build and Run the C++ Client
cd client_cpp
mkdir build && cd build
cmake ..
make
./client

---

## Purpose
This project is ideal for:

1. Learning about TCP networking
2. Experimenting with cross-language communication
3. Using as a foundation for more advanced protocols
4. Teaching basic client–server architecture

---

