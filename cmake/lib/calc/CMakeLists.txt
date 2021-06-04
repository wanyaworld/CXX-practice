project(calc)

add_library(${PROJECT_NAME} calc.cpp)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)
