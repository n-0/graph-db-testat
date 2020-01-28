cmake_minimum_required(VERSION 3.15)
project(graph_db_testat C)

set(CMAKE_C_STANDARD 99)

add_executable(graph_db_testat main.c graph.c graph.h ops.c ops.h hashmap.c hashmap.h)