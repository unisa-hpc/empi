# Copyright (c) 2022-2023 University of Salerno, Italy. All rights reserved.

macro(create_example TARGET_NAME SRC_FILE)
    add_executable(${TARGET_NAME} ${SRC_FILE})
    target_link_libraries(${TARGET_NAME} INTERFACE empi)
endmacro()

option(BUILD_MPI_EXAMPLES "Build MPI examples" OFF)
option(BUILD_UMPI_EXAMPLES "Build UMPI examples" OFF)
option(BUILD_MPL_EXAMPLES "Build MPL examples" OFF)


add_subdirectory(all_reduce)
add_subdirectory(bcast)
add_subdirectory(bdring)
add_subdirectory(ibcast)
add_subdirectory(ping_pong)
add_subdirectory(vibrating_string)