cmake_minimum_required(VERSION 3.6)
project(mq_apicall_exit_c)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

INCLUDE_DIRECTORIES(/opt/mqm/inc)
LINK_DIRECTORIES(/opt/mqm/lib64)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(SOURCE_FILES amqsmsolog.c)

ADD_LIBRARY(smsolog SHARED ${SOURCE_FILES})