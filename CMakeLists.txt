cmake_minimum_required(VERSION 3.0)

project (HTTP_OCR)
#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -lboost_system -lboost_filesystem -lboost_thread-mt -lboost_locale-mt -lssl -lcrypto -lpthread")

include_directories(${Boost_INCLUDE_DIRS})
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.65 COMPONENTS thread system REQUIRED)

#message("Boost include dir: ${Boost_INCLUDE_DIR}")
#message("Boost libraries: ${Boost_LIBRARIES}")
#include_directories(${PROJECT_SOURCE_DIR}/components/common/boost)

include_directories(/usr/local/opt/openssl/include)
find_library(SSL_LIB libssl.a REQUIRED)
find_library(CRYPTO_LIB libcrypto.a REQUIRED)

set (OPENSSL_LIBRARY_DIRS /usr/local/opt/openssl/lib)
link_directories(${OPENSSL_LIBRARY_DIRS})


set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

include_directories(/usr/lib)
find_package(Threads REQUIRED)
#message("Threads libraries/flag: ${CMAKE_THREAD_LIBS_INIT}")


add_subdirectory(components)
link_directories(${PROJECT_SOURCE_DIR}/components/common/libpng)

#dependancy 

##boost
#openssl

##leptonica
#/usr/local/include/leptonica
#/usr/local/opt/leptonica/lib/
