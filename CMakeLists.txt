cmake_minimum_required(VERSION 3.0.0)
project(empi-project VERSION 0.1.0)

include(CTest)
enable_testing()
set(CMAKE_CXX_STANDARD 20)
include(FindMPI REQUIRED)

option(MPI_EXAMPLES "Enable MPI examples" OFF)
option(BUILD_MPL "Build MPL" OFF)
option(ENABLE_TESTS "Enable testing" OFF)

set(header_files
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/empi.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/context.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/type_traits.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_grp_hdl.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/tag.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/utils.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_group.hpp 
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/request_pool.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/empi_defines.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/request.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/async_event.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/datatype.hpp
	)

add_library(empi INTERFACE ${header_files})
target_include_directories(empi INTERFACE MPI::MPI_CXX_INCLUDE_DIRS)

set_target_properties(empi
	PROPERTIES
	PUBLIC_HEADER "${header_files}"
)

target_include_directories(empi
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(empi INTERFACE MPI::MPI_CXX)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build" FORCE)
	message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

if(ENABLE_TESTS)
	enable_testing()
	add_subdirectory(lib/Catch2) # Find Catch2
	add_subdirectory(test)
endif()

if(BUILD_MPL)
	add_subdirectory(lib/mpl-subset)
endif()

add_subdirectory(examples)