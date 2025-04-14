cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(SW.Module.Logger LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCES})
endif()

add_subdirectory(vendor/quill)
target_link_libraries(${PROJECT_NAME} PUBLIC quill::quill)


include(ExternalProject)

ExternalProject_Add(spdlog
    PREFIX spdlog
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/spdlog
    CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    -DSPDLOG_BUILD_SHARED=OFF
    INSTALL_COMMAND ""
)

add_dependencies(${PROJECT_NAME} spdlog)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/vendor/spdlog/include)

target_include_directories(${PROJECT_NAME} PUBLIC ../SW.Module.Common/src)
