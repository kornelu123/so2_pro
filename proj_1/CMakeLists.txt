cmake_minimum_required(VERSION 3.13)

project(dinner_time C)

set(CMAKE_C_STANDARD 11)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME}
        main.c
        )

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
