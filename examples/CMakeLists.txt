macro(create_example TARGET_NAME SRC_FILE)
    add_executable(${TARGET_NAME} ${SRC_FILE})
    target_link_libraries(${TARGET_NAME} PUBLIC empi)
endmacro()

if(MPI_EXAMPLES)
    create_example(mpi_test_send_recv mpi_test_send_recv.cpp)
    create_example(mpi_test_usend_urecv mpi_test_usend_urecv.cpp)
    create_example(mpi_test_isend_irecv mpi_test_isend_irecv.cpp)
    create_example(mpi_test_iusend_iurecv mpi_test_iusend_iurecv.cpp)

    create_example(mpi_bdring_isend_irecv bdring/mpi_bdring_isend_irecv.cpp)
    create_example(mpi_bdring_iusend_iurecv bdring/mpi_bdring_iusend_iurecv.cpp)

endif()

create_example(empi_send_recv empi_send_recv.cpp)
create_example(empi_submit_async empi_submit_async.cpp)
create_example(empi_submit empi_submit.cpp)
create_example(empi_bidir_ring empi_bidir_ring.cpp)
create_example(empi_ibcast empi_ibcast_allreduce.cpp)
create_example(empi_test_send_recv empi_test_send_recv.cpp)
create_example(empi_test_isend_irecv empi_test_isend_irecv.cpp)
create_example(empi_bdring_isend_irecv bdring/empi_bdring_isend_irecv.cpp)

create_example(mpl_bdring_isend_irecv bdring/mpl_bdring_isend_irecv.cpp)