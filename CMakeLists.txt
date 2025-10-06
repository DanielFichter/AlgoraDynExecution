cmake_minimum_required(VERSION 3.15)
project(algoraDynExecution)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp src/cli.cpp src/algorithmtype.cpp src/measureperformance.cpp src/testcorrectness.cpp src/booltostring.cpp src/io.cpp)

add_library(AlgoraCore STATIC IMPORTED)
set_target_properties(AlgoraCore PROPERTIES
    IMPORTED_LOCATION "${ALGORACORE_ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/libAlgoraCore.a"
    INTERFACE_INCLUDE_DIRECTORIES "${ALGORACORE_ROOT_DIR}/src"
)

add_library(AlgoraDyn STATIC IMPORTED)
set_target_properties(AlgoraDyn PROPERTIES
    IMPORTED_LOCATION "${ALGORADYN_ROOT_DIR}/build/${CMAKE_BUILD_TYPE}/libAlgoraDyn.a"
    INTERFACE_INCLUDE_DIRECTORIES "${ALGORADYN_ROOT_DIR}/src"
)

target_link_libraries(${PROJECT_NAME} PUBLIC AlgoraDyn AlgoraCore)