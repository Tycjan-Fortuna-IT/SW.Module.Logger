cmake_minimum_required(VERSION 3.16...3.26 FATAL_ERROR)

project(Logger VERSION 0.2.1 LANGUAGES CXX)

if(WIN32)
    add_compile_definitions(SW_PLATFORM_WINDOWS)
elseif(UNIX)
    add_compile_definitions(SW_PLATFORM_LINUX)
endif()

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
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install/spdlog
    -DSPDLOG_BUILD_SHARED=OFF
)

add_dependencies(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/install/spdlog/include)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_COMPILED_LIB)

find_library(SPDLOG_LIBRARY
    NAMES spdlog spdlogd # Look for both Release and Debug versions
    PATHS ${CMAKE_BINARY_DIR}/install/spdlog/lib
    NO_DEFAULT_PATH
)

if(NOT SPDLOG_LIBRARY)
    message(FATAL_ERROR "spdlog library not found!")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${SPDLOG_LIBRARY})

if(SW_LOGGER_COMPILE_TESTS)
    add_executable(compile_test tests/test.cpp)
    target_link_libraries(compile_test PRIVATE Logger)

    enable_testing()
    add_test(NAME compile_test COMMAND $<TARGET_FILE:compile_test>)
endif()
