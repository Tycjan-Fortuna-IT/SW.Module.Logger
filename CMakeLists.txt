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

set(SPDLOG_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/spdlog/src/spdlog-build")

if(MSVC)
    set(SPDLOG_LIB_DIR "${SPDLOG_LIB_DIR}/$<CONFIG>") # Append Debug or Release for MSVC
endif()

if(WIN32)
    set(SPDLOG_LIB_DIR "$<IF:$<CONFIG:Debug>,${SPDLOG_LIB_DIR}/spdlogd.lib,${SPDLOG_LIB_DIR}/spdlog.lib>")
else()
    set(SPDLOG_LIB_DIR "$<IF:$<CONFIG:Debug>,${SPDLOG_LIB_DIR}/libspdlogd.a,${SPDLOG_LIB_DIR}/libspdlog.a>")
endif()

ExternalProject_Add(spdlog
    PREFIX spdlog
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/spdlog
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DSPDLOG_BUILD_SHARED=OFF
    BUILD_BYPRODUCTS ${SPDLOG_LIB_DIR}
    INSTALL_COMMAND ""
)

add_dependencies(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/spdlog/include)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_COMPILED_LIB)

# Link against the deduced SPDLOG_LIB_DIR
target_link_libraries(${PROJECT_NAME} PUBLIC ${SPDLOG_LIB_DIR})

if(SW_LOGGER_COMPILE_TESTS)
    add_executable(compile_test tests/test.cpp)
    target_link_libraries(compile_test PRIVATE Logger)

    enable_testing()
    add_test(NAME compile_test COMMAND $<TARGET_FILE:compile_test>)
endif()
