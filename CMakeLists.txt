cmake_minimum_required(VERSION 2.8.12)

project(Challenge)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Qt5Core)

set(TARGET_H
    sqlite3.h
    database.h
)
set (SOURCES
    main.cpp
    sqlite3.c
    database.c
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Core ${CMAKE_DL_LIBS})

