cmake_minimum_required(VERSION 3.10)

project(MyLibNN)

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.6)
set(CMAKE_CXX_FLAGS "-Wall -fPIC")

find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} pywrap.cpp)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYBIND11_INCLUDE_DIRS})

# Release by default
# Turn on Debug with "-DCMAKE_BUILD_TYPE=Debug"
if( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release )
endif()

list(APPEND CMAKE_PREFIX_PATH "/home/donald/Documents/boost_1_83_0")
find_package(Boost)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  # ADD_EXECUTABLE(octree_nn_search src/octree_nn_search.cpp)
endif()

# find gtest ...
IF(IS_DIRECTORY "/usr/src/gtest/")
  MESSAGE("Found google test sources in /usr/src/gtest/")
  
  ADD_SUBDIRECTORY(/usr/src/gtest/ gtest)
  
  # mimick the behaviour of find_package(GTest)
  SET(GTEST_FOUND TRUE)
  SET(GTEST_BOTH_LIBRARIES gtest gtest_main)
  SET(GTEST_LIBRARIES gtest)
  SET(GTEST_MAIN_LIBRARIES gtest_main)
ELSE()
  FIND_PACKAGE(GTest)
ENDIF()

# build gtests if possible
IF(GTEST_FOUND)
  MESSAGE("Building tests with google test framework")
	ENABLE_TESTING()
	INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
	# ADD_EXECUTABLE(octree-test test/octree-test.cpp)
	# TARGET_LINK_LIBRARIES(octree-test ${GTEST_MAIN_LIBRARIES})
	ADD_TEST(octree-test octree-test)
ENDIF()