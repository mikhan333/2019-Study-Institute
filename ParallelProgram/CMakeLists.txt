cmake_minimum_required(VERSION 3.1)

project(project_name)

find_library(PTHREAD_LIBRARY pthread)
find_library(PCRE_LIBRARY pcre)

include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

file( GLOB LIB_SOURCES base/*.cpp required/integral_lab2.cpp)
file( GLOB LIB_HEADERS base/*.h )

add_library( YourLib ${LIB_SOURCES} ${LIB_HEADERS} )
#set(SOURCES base/hello_pth.cpp base/sum_pth.cpp)
#add_executable(main ${SOURCES})
#add_executable(main base/transfer_pth.cpp)
add_executable(main required/integral_lab2.cpp)

target_link_libraries(main ${PTHREAD_LIBRARY} ${PCRE_LIBRARY})