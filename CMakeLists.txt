cmake_minimum_required(VERSION 3.15)
project(graph_db_testat C)

set(CMAKE_C_STANDARD 99)

find_package(LibXml2 REQUIRED)

add_executable(graph_db_testat main.c graph.c graph.h ops.c ops.h hashmap.c hashmap.h storage.c storage.h)
target_compile_definitions(graph_db_testat PUBLIC ${LIBXML2_DEFINITIONS})
target_include_directories(graph_db_testat PUBLIC ${LIBXML2_INCLUDE_DIRS})
target_link_libraries(graph_db_testat ${LIBXML2_LIBRARIES})
