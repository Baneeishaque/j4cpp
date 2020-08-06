cmake_minimum_required(VERSION 3.15)
project(j4cpp)

#set(JAVA_HOME C:/apps/openjdk-11.28_x64) # ?
#set(JAVA_HOME C:/apps/java-1.8.0-openjdk-1.8.0.181-1.b13.ojdkbuild.windows.x86_64)
#set(JAVA_HOME C:/apps/java-11-openjdk-11.0.7.10-1.windows.ojdkbuild.x86_64)

#set(JAVA_HOME C:/apps/jdk-14.0.2)# https://jdk.java.net/14/
#set(JRE_HOME ${JAVA_HOME})

set(JAVA_HOME "C:/Program Files/Java/jdk1.8.0_211")
set(JRE_HOME "${JAVA_HOME}/jre")

message("JAVA_HOME: ${JAVA_HOME}")

set(CMAKE_CXX_STANDARD 14)

add_executable(j4cpp main.cpp)
add_executable(hello hello.cpp)

include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)
include_directories(${JAVA_HOME}/include/win32/bridge)

file(COPY ${JRE_HOME} DESTINATION .)
