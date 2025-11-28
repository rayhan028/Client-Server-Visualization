cmake_minimum_required(VERSION 3.10)
project(AdvancedClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    main.cpp
    TcpClient.cpp
)

add_executable(client ${SOURCES})

target_include_directories(client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
