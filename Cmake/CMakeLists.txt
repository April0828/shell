cmake_minimum_required(VERSION 3.0)
project(cmakelist)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 11)

add_executable(demo "builtin.cpp" "controlflow.cpp" "execute.cpp" "process2.cpp"
                    "smsh2.cpp" "splitline.cpp" "varlib.cpp" "smsh.h" "varlib.h")