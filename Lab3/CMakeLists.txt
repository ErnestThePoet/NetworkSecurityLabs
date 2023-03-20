cmake_minimum_required(VERSION 3.22)
project(LibnetDemo)
aux_source_directory(./common/ COMMON_SRCS)
aux_source_directory(./sender/ SENDER_SRCS)
aux_source_directory(./receiver/ RECEIVER_SRCS)

add_executable(sender ${SENDER_SRCS} ${COMMON_SRCS})
add_executable(receiver ${RECEIVER_SRCS} ${COMMON_SRCS})

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-O2")

target_link_libraries(sender net)
target_link_libraries(receiver net)