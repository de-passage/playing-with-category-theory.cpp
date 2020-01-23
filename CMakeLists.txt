cmake_minimum_required(VERSION 3.0.0)
project(category_theory VERSION 0.1.0)

include(CTest)
enable_testing()

set(INCLUDE_DIRECTORY category_theory)
set(UTILITY_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/utils)
set(CONSTRUCTIONS_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/constructions)
set(STLADAPTERS_INCLUDE_DIRECTORY ${INCLUDE_DIRECTORY}/stl)
set(HEADER_FILES 
    ${INCLUDE_DIRECTORY}/category_theory.hpp 
    ${INCLUDE_DIRECTORY}/applicative.hpp 
    ${INCLUDE_DIRECTORY}/functor.hpp 
    ${INCLUDE_DIRECTORY}/monad.hpp 
    ${UTILITY_INCLUDE_DIRECTORY}/functor.hpp 
    ${UTILITY_INCLUDE_DIRECTORY}/monad.hpp 
    ${UTILITY_INCLUDE_DIRECTORY}/utils.hpp 
    ${CONSTRUCTIONS_INCLUDE_DIRECTORY}/functor.hpp 
    ${CONSTRUCTIONS_INCLUDE_DIRECTORY}/applicative.hpp 
    ${CONSTRUCTIONS_INCLUDE_DIRECTORY}/monad.hpp 
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

###############
# Google Test #
###############

# Download and unpack googletest at configure time
configure_file(gtest.CMakeLists.txt googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(isomorphisms tests/isomorphisms.cpp)
target_link_libraries(isomorphisms gtest_main)
add_test(NAME isomorphism_test COMMAND isomorphisms)