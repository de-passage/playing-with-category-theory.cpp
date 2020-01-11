cmake_minimum_required(VERSION 3.0.0)
project(category_theory VERSION 0.1.0)

include(CTest)
enable_testing()

set(INCLUDE_DIRECTORY category_theory)
set(UTILITY_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/utils)
set(CONSTRUCTIONS_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/constructions)
set(STLADAPTERS_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/stl)
set(HEADER_FILES 
    ${INCLUDE_DIRECTORY}/functor.hpp 
    ${UTILITY_INCLUDE_DIRECTORY}/functor.hpp 
    ${UTILITY_INCLUDE_DIRECTORY}/utils.hpp 
    ${CONSTRUCTIONS_INCLUDE_DIRECTORY}/functor.hpp 
    ${STLADAPTERS_INCLUDE_DIRECTORY}/functor.hpp)

add_executable(category_theory main.cpp ${HEADER_FILES})
target_include_directories(category_theory PRIVATE ${INCLUDE_DIRECTORY})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_STANDARD 17)
include(CPack)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)