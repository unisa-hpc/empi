macro(create_example TARGET_NAME SRC_FILE)
    add_executable(${TARGET_NAME} ${SRC_FILE})
    target_link_libraries(${TARGET_NAME} PUBLIC empi)
endmacro()

create_example(empi_send_recv empi_send_recv.cpp)
create_example(empi_submit empi_submit.cpp)