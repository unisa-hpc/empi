cmake_minimum_required(VERSION 3.0)

project(LULESH CXX)
set(EXPORT_COMPILE_COMMANDS TRUE)

option(WITH_MPI    "Build LULESH with MPI"          TRUE)
option(WITH_OPENMP "Build LULESH with OpenMP"       TRUE)
option(WITH_SILO   "Build LULESH with silo support" FALSE)
option(USE_MPL  "BUILD LULESH with MPL"  FALSE)
option(USE_EMPI  "BUILD LULESH with MPL"  FALSE)
set(EMPI_PATH "../../include" CACHE STRING "PATH TO EMPI")

set(LULESH_SOURCES
  lulesh-comm.cc
  lulesh-init.cc
  lulesh-util.cc
  lulesh-viz.cc
  lulesh.cc)

set(LULESH_EXEC lulesh2.0)

if(USE_EMPI)
  set(CMAKE_CXX_STANDARD 20)
endif()

add_executable(${LULESH_EXEC} ${LULESH_SOURCES})

if (WITH_MPI)
find_package(MPI REQUIRED)
target_include_directories(${LULESH_EXEC} PUBLIC ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH})
target_compile_definitions(${LULESH_EXEC} PUBLIC "-DUSE_MPI=1")
list(APPEND LULESH_EXTERNAL_LIBS ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES})

if(USE_MPL)
  if(USE_EMPI)
  message(FATAL_ERROR "Cannot enable both MPL and EMPI implementation...")
endif()
endif()

if(USE_MPL)
  target_compile_definitions(${LULESH_EXEC} PUBLIC "-DUSE_MPL_CXX=1")
endif()

if(USE_EMPI)
  target_include_directories(${LULESH_EXEC} PUBLIC "${EMPI_PATH}")
  target_compile_definitions(${LULESH_EXEC} PUBLIC "-DUSE_EMPI=1")
endif()

else()
  add_definitions("-DUSE_MPI=0")
endif()

if (WITH_OPENMP)
  find_package(OpenMP REQUIRED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if (WITH_SILO)
  find_path(SILO_INCLUDE_DIR silo.h
    HINTS ${SILO_DIR}/include)
  find_library(SILO_LIBRARY
    NAMES siloh5
    HINTS ${SILO_DIR}/lib)

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(SILO DEFAULT_MSG
    SILO_LIBRARY
    SILO_INCLUDE_DIR)

  if (SILO_FOUND)
    add_definitions("-DVIZ_MESH")
    include_directories(${SILO_INCLUDE_DIR})
    #   Note: silo needs to be built as a dynamic lib, otherwise 
    # there are additional dependencies (hdf5) which we don't know.
    # This would be fixed by silo providing a CMake package.
    list(APPEND LULESH_EXTERNAL_LIBS ${SILO_LIBRARY})
  endif()
endif()

target_link_libraries(${LULESH_EXEC} INTERFACE ${LULESH_EXTERNAL_LIBS})