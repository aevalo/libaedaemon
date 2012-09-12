cmake_minimum_required(VERSION 2.8)
project (libAeDaemon C)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

include(EnsureOutOfSourceBuild)
find_package(Apr)
find_package(AprUtil)

add_definitions(${LIBAPR_DEFINITIONS} ${LIBAPR-UTIL_DEFINITIONS})
include_directories(${LIBAPR_INCLUDE_DIRS} ${LIBAPR-UTIL_INCLUDE_DIRS})

add_executable(aedaemon src/main.c)
target_link_libraries(aedaemon ${LIBAPR_LIBRARIES} ${LIBAPR-UTIL_LIBRARIES})
