
set(SRC_LIST
   async_client.cpp
)

link_libraries(libpng.a)

link_libraries(liblept.a)
add_executable(http-client ${SRC_LIST})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/..)

