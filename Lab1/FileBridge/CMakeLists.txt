cmake_minimum_required(VERSION 3.22)
project(FileBridge)
aux_source_directory(./common SRCS_COMMON)
aux_source_directory(./client SRCS_CLIENT)
aux_source_directory(./server SRCS_SERVER)

add_executable(client ${SRCS_CLIENT} ${SRCS_COMMON})
add_executable(server ${SRCS_SERVER} ${SRCS_COMMON})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O2")