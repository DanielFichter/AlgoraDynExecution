cmake_minimum_required(VERSION 3.15)
project(algoraDynTests)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} main.cpp)

#add_library(AlgoraCore STATIC ../AlgoraCore/src/Debug/)
#add_library(AlgoraDyn STATIC ../AlgoraDynOptimization/src/Debug/)

add_library(AlgoraCore STATIC IMPORTED)
set_target_properties(AlgoraCore PROPERTIES
    IMPORTED_LOCATION "/home/daniel-fichter/algora/AlgoraCore/build/Debug/libAlgoraCore.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/daniel-fichter/algora/AlgoraCore/src"
)

add_library(AlgoraDyn STATIC IMPORTED)
set_target_properties(AlgoraDyn PROPERTIES
    IMPORTED_LOCATION "/home/daniel-fichter/algora/AlgoraDynOptimization/build/Debug/libAlgoraDyn.a"
    INTERFACE_INCLUDE_DIRECTORIES "/home/daniel-fichter/algora/AlgoraDynOptimization/src"
)


target_link_libraries(${PROJECT_NAME} PUBLIC AlgoraDyn AlgoraCore)
#target_include_directories(algoraDynTests PUBLIC ${PROJECT_SOURCE_DIR}/../AlgoraDynOptimization/src ${PROJECT_SOURCE_DIR}/../AlgoraCore/src)