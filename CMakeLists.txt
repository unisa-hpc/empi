cmake_minimum_required(VERSION 3.0.0)
project(empi-project VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)

include(FindMPI REQUIRED)


set(header_files
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/empi.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/context.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/type_traits.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_grp_hdl.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/tag.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/utils.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/empi/message_group.hpp include/empi/empi_defines.hpp include/empi/request.hpp include/empi/async_event.hpp include/empi/datatype.hpp)

add_library(empi INTERFACE ${header_files})
target_include_directories(empi INTERFACE MPI::MPI_CXX_INCLUDE_DIRS)

set_target_properties(empi
	PROPERTIES
	PUBLIC_HEADER "${header_files}"
)

target_include_directories(empi
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(empi INTERFACE MPI::MPI_CXX)
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "The type of build" FORCE)
	message(STATUS "Setting build type to '${CMAKE_BUILD_TYPE}' as none was specified")
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()


add_subdirectory(examples)