cmake_minimum_required(VERSION 3.22)
project(Capturer)
aux_source_directory(./ SRCS)

add_executable(capturer ${SRCS})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O2")

target_link_libraries(capturer pcap)