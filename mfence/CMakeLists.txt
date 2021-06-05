add_executable(fence fence.cpp)
add_executable(no_fence no_fence.cpp)
target_link_libraries(fence pthread)
target_link_libraries(no_fence pthread)
