# 
# Copyright (c) 2022-2023 University of Salerno, Italy. All rights reserved.
# 

cmake_minimum_required(VERSION 3.0.0)

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
  cmake_policy(SET CMP0135 NEW) # ExternalProject downloads will have archive timestamps
endif()

set(EMPI_VERSION 0.1.0)
set(EMPI_VERSION_MAJOR 0)
set(EMPI_VERSION_MINOR 1)
set(EMPI_VERSION_PATCH 0)

project(empi-project VERSION "${EMPI_VERSION}" LANGUAGES CXX)	

set(CMAKE_CXX_STANDARD 20)
include(FindMPI REQUIRED)

option(ENABLE_UNCHECKED_FUNCTION "Enable unchecked function (requires custom OMPI)" ON)
option(BUILD_EXAMPLES "Build Examples" OFF)
option(ENABLE_TESTS "Enable testing" OFF)

# Default build flags
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -fno-omit-frame-pointer" CACHE STRING "Flags used by the C++ compiler during debug builds." FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -DNDEBUG -march=native -ffast-math -g -fno-omit-frame-pointer" CACHE STRING "Flags used by the C++ compiler during release builds with debug info." FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -ffast-math" CACHE STRING "Flags used by the C++ compiler during release builds." FORCE)

set(CONFIG_PATH "${CMAKE_CURRENT_BINARY_DIR}/include/empi/config.hpp")
configure_file(
	"${PROJECT_SOURCE_DIR}/include/empi/config.hpp.in"
	${CONFIG_PATH}
	@ONLY
)

set(header_files
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/empi.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/context.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/type_traits.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_grp_hdl.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/tag.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/utils.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_group.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/request_pool.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/request.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/async_event.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/datatype.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/defines.hpp
	${CONFIG_PATH}
	)

add_library(empi INTERFACE ${header_files})

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_BINARY_DIR}/include
	MPI::MPI_CXX_INCLUDE_DIRS
)

target_link_libraries(empi INTERFACE MPI::MPI_CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build" FORCE)
	message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

# Force color diagnostics on Ninja
if(CMAKE_GENERATOR STREQUAL "Ninja")
	target_compile_options(empi INTERFACE -fdiagnostics-color=always)
endif()


if(ENABLE_TESTS)
	include(CTest)
	enable_testing()
	add_subdirectory(lib/Catch2) # Find Catch2
	add_subdirectory(test)
endif()

if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

message("//===---------------------------------------===//")
message("//           CONFIGURATION SUMMARY             //")
message("//===---------------------------------------===//")
message("-> CMAKE_BUILD_TYPE:.................. ${CMAKE_BUILD_TYPE}")
message("-> CMAKE_CXX_COMPILER:................ ${CMAKE_CXX_COMPILER}")
message("-> ENABLE_UNCHECKED_FUNCTION:........ ${ENABLE_UNCHECKED_FUNCTION}")
message("-> BUILD_EXAMPLES:................... ${BUILD_EXAMPLES}")
if(BUILD_EXAMPLES)
message("-> BUILD_MPI_EXAMPLES:............... ${BUILD_MPI_EXAMPLES}")
message("-> BUILD_UMPI_EXAMPLES:.............. ${BUILD_EXAMPLES}")
message("-> BUILD_MPL_EXAMPLES:............... ${BUILD_MPL_EXAMPLES}")
endif()
message("-> ENABLE_TESTS:..................... ${ENABLE_TESTS}")
message("//===---------------------------------------===//")
