cmake_minimum_required(VERSION 3.15)
project(j4cpp)

set(JAVA_HOME C:/apps/java-1.8.0-openjdk-1.8.0.181-1.b13.ojdkbuild.windows.x86_64)
set(JRE_HOME c:/apps/java-14-openjdk-jre-14.0.1.7-1.windows.ojdkbuild.x86_64)

#set(JAVA_HOME C:/apps/openjdk-11.28_x64) # ?
#set(JAVA_HOME C:/apps/java-11-openjdk-11.0.7.10-1.windows.ojdkbuild.x86_64)
#set(JAVA_HOME C:/apps/jdk-14.0.2)# https://jdk.java.net/14/
#set(JAVA_HOME "C:/Program Files/Java/jdk1.8.0_211")
#set(JRE_HOME ${JAVA_HOME})
#set(JRE_HOME "${JAVA_HOME}/jre")

message("JAVA_HOME: ${JAVA_HOME}")

set(CMAKE_CXX_STANDARD 14)

add_executable(j4cpp showcase/j4cpp.cpp)

add_executable(j4cs_test j4cs_test/j4cs_test.cpp)

add_library(j4cs SHARED j4cs/j4cs.cpp)
#add_executable(j4cs j4cs/j4cs.cpp)

include_directories(${JAVA_HOME}/include)
include_directories(${JAVA_HOME}/include/win32)
include_directories(${JAVA_HOME}/include/win32/bridge)

file(GLOB JRE_FILES "${JRE_HOME}/*")
if (${CMAKE_GENERATOR} STREQUAL "Visual Studio 17 2022")
    file(COPY ${JRE_FILES} DESTINATION Debug/jre)
else ()
    file(COPY ${JRE_FILES} DESTINATION jre)
endif ()

file(COPY showcase/J4Cpp.class DESTINATION classes)
file(COPY j4cs_test/J4Cs.class DESTINATION classes)
