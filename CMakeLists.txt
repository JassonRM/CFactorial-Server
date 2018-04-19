cmake_minimum_required(VERSION 3.9)
project(CFactorial_Server)

set(CMAKE_CXX_STANDARD 11)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.66.0/include)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.66.0/lib)

set(RapidJSON_INCLUDE_DIR /usr/local/Cellar/rapidjson/1.1.0/include)
set(RapidJSON_LIBRARY_DIR /usr/local/Cellar/rapidjson/1.1.0/lib)



find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

find_package(RapidJSON)
include_directories(${RapidJSON_INCLUDE_DIR})
link_directories(${RapidJSON_LIBRARY_DIR})

add_executable(CFactorial_Server main.cpp Server.cpp Server.h Session.cpp Session.h MemoryManager.cpp MemoryManager.h Variable.h)

target_link_libraries(CFactorial_Server ${Boost_SYSTEM_LIBRARY})