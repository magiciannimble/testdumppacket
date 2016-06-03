project(TestDumpPacket)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -m32")
set(CMAKE_C_FLAGS "-m32")

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
