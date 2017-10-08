
set(SRC_LIST
        main.cpp
        io_service_pool.cpp
        mime_types.cpp
        reply.cpp
        server.cpp
        picohttpparser.c
        utils.cpp
        request.cpp
        multipart_parser.c
        websocket.cpp
)

add_executable(http-ocr ${SRC_LIST})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/..)