cmake_minimum_required(VERSION 3.15)
project(j4cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(j4cpp main.cpp)

include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)
include_directories(${JAVA_HOME}/include/win32/bridge)

find_library(JVM_AS_LIBRARY jvm ${JAVA_HOME}/bin/server)
target_link_libraries(j4cpp ${JVM_AS_LIBRARY})

file(GLOB JVM_EXTRA_DLLS "${JAVA_HOME}/bin/*.dll")
file(GLOB JVM_MAIN_DLLS "${JAVA_HOME}/bin/server/*.dll")
file(COPY ${JVM_EXTRA_DLLS} DESTINATION .)
file(COPY ${JVM_MAIN_DLLS} DESTINATION .)