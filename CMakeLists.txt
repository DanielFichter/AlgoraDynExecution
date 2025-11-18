cmake_minimum_required(VERSION 3.15)
project(algoraDynExecution)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-O -march=x86-64 -mtune=generic -flto=auto -fno-inline")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-O3 -march=native -mtune=native -flto=auto")
endif()

add_executable(${PROJECT_NAME} src/main.cpp src/algoracli.cpp src/algorithmtype.cpp src/measureperformance.cpp src/testcorrectness.cpp src/booltostring.cpp src/io.cpp src/unit_test.cpp src/algorithmexecuter.cpp src/algorithmsettings.cpp src/createalgorithmsettings.cpp src/erdosrenyi_generator.cpp src/graphreader.cpp src/randomgraphinstantiator.cpp)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(cli11_proj QUIET GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git GIT_TAG v2.3.2)
FetchContent_MakeAvailable(cli11_proj)

add_library(AlgoraCore STATIC IMPORTED)
set_target_properties(AlgoraCore PROPERTIES
    IMPORTED_LOCATION "${ALGORACORE_ROOT_DIR}/build/${ALGORA_BUILD_TYPE}/libAlgoraCore.a"
    INTERFACE_INCLUDE_DIRECTORIES "${ALGORACORE_ROOT_DIR}/src"
)

add_library(AlgoraDyn STATIC IMPORTED)
set_target_properties(AlgoraDyn PROPERTIES
    IMPORTED_LOCATION "${ALGORADYN_ROOT_DIR}/build/${ALGORA_BUILD_TYPE}/libAlgoraDyn.a"
    INTERFACE_INCLUDE_DIRECTORIES "${ALGORADYN_ROOT_DIR}/src"
)

target_link_libraries(${PROJECT_NAME} PUBLIC AlgoraDyn AlgoraCore)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE CLI11::CLI11)