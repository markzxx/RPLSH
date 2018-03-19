cmake_minimum_required(VERSION 2.8)

project(rplsh)
set(CMAKE_CXX_STANDARD 11)
include_directories(${PROJECT_SOURCE_DIR}/general)
include_directories(${PROJECT_SOURCE_DIR}/algorithm)
include_directories(${PROJECT_SOURCE_DIR})

#OpenMP
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else()
    message(FATAL_ERROR "no OpenMP supprot")
endif()

#OpenBLAS
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/module)
find_package(OpenBLAS REQUIRED)
include_directories(${OpenBLAS_INCLUDE_DIR})
set(BLAS_LIB ${OpenBLAS_LIB})

add_definitions (-std=c++11 -O3 -lboost -lopenblas -march=native -Wall -DINFO)
add_executable(index samples/index.cc)
add_executable(search samples/search.cc)
#target_link_libraries(index ${PROJECT_NAME})
#target_link_libraries(search ${PROJECT_NAME})
#
#file(GLOB_RECURSE CPP_SOURCES *.cc)
#
#add_library(${PROJECT_NAME} ${CPP_SOURCES})
#add_library(${PROJECT_NAME}_s STATIC ${CPP_SOURCES})