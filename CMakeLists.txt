cmake_minimum_required(VERSION 3.16...3.26 FATAL_ERROR)

project(Logger VERSION 0.2.1 LANGUAGES CXX)

if(SW_LOGGER_USE_FMT)
    add_compile_definitions(SW_LOGGER_USE_FMT)
endif()

include(ExternalProject)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE HEADERS "src/Logger/*.hpp")
file(GLOB_RECURSE SOURCES "src/Logger/*.cpp")

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

ExternalProject_Add(spdlog
    PREFIX spdlog
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/spdlog
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
    -DSPDLOG_BUILD_SHARED=OFF
)

add_dependencies(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/install/include)
