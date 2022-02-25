cmake_minimum_required(VERSION 3.16)

project(graph_serial)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS serialized_graph.proto)

add_executable(${PROJECT_NAME} main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(${PROJECT_NAME}
    ${PROTOBUF_LIBRARY}
    pthread
)
