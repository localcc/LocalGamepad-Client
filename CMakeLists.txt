cmake_minimum_required(VERSION 3.15)
project(LocalGamepad_Client)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(NOT DEFINED SDL2_PATH)
    set(SDL2_PATH "C:\\SDL2")
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(udp_client_src
        ${CMAKE_CURRENT_SOURCE_DIR}/include/udp_client/udp_client.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/udp_client/udp_client.cpp)

set(sdl_gamepad_src
${CMAKE_CURRENT_SOURCE_DIR}/include/sdl_gamepad/sdl_gamepad.h
${CMAKE_CURRENT_SOURCE_DIR}/src/sdl_gamepad/sdl_gamepad.cpp)


add_executable(${PROJECT_NAME} main.cpp ${udp_client_src} ${sdl_gamepad_src})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 wsock32)
endif()