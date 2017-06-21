cmake_minimum_required(VERSION 3.3)
project(pals)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    applications/app.c
    applications/conf.c
    applications/conf_mrate.c
    applications/mrate0.c
    applications/mrate1.c
    app.h
    conf.h
    pals_base.h
    pals_conf_helper.c
    pals_main.c
    pals_mcast_socket.c
    pals_mcast_socket.h
    pals_msg.c
    pals_msg.h
    pals_port.c
    pals_port.h
    pals_read_msg.c
    pals_read_msg.h
    pals_recv.c
    pals_send.c
    pals_socket.c
    pals_socket.h
    pals_task.h
    pals_time.c
    pals_time.h
    pals_timer.c
    pals_timer.h)

add_executable(pals ${SOURCE_FILES})