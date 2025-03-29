cmake_minimum_required(VERSION 3.15)
project(algoraDynExecution)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

add_library(AlgoraCore STATIC IMPORTED)
set_target_properties(AlgoraCore PROPERTIES
    IMPORTED_LOCATION "${ALGORACORE_ROOT_DIR}/build/Debug/libAlgoraCore.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/daniel-fichter/algora/AlgoraCore/src"
)

add_library(AlgoraDyn STATIC IMPORTED)
set_target_properties(AlgoraDyn PROPERTIES
    IMPORTED_LOCATION "${ALGORADYN_ROOT_DIR}/build/Debug/libAlgoraDyn.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/daniel-fichter/algora/AlgoraDynOptimization/src"
)

target_link_libraries(${PROJECT_NAME} PUBLIC AlgoraDyn AlgoraCore)