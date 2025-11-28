# Simple Client–Server Application  
### Python TCP Server + C++ TCP Client

This repository demonstrates a minimal, educational client–server system implemented using two different programming languages:

- **Python** for the server  
- **C++** for the client  

The goal is to provide a clean, easy-to-understand example of TCP socket communication across languages.

---

## Features

- **TCP communication** between Python and C++
- **Threaded Python server** (handles multiple clients)
- **Interactive C++ client**
- **Simple command protocol**
  - `PING` → responds with `PONG`
  - `TIME` → returns current server time
  - `EXIT` → closes connection
- **Beginner-friendly, extensible design**

---

## Repository Structure

sample-client-server/
│
├── server_python/
│ ├── server.py
│ └── requirements.txt
│
└── client_cpp/
├── main.cpp
├── CMakeLists.txt
└── README.md


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

cd client_cpp
mkdir build && cd build
cmake ..
make
./client

## Purpose

This project is ideal for:

learning about TCP networking

experimenting with cross-language communication

using as a foundation for more advanced protocols

teaching basic client–server architecture
