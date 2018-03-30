cmake_minimum_required(VERSION 3.9)
project(CFactorial_Server)

set(CMAKE_CXX_STANDARD 11)

set(Boost_INCLUDE_DIR /usr/local/Cellar/boost/1.66.0/include)
set(Boost_LIBRARY_DIR /usr/local/Cellar/boost/1.66.0/lib)


find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(CFactorial_Server main.cpp)

target_link_libraries(CFactorial_Server ${Boost_SYSTEM_LIBRARY})