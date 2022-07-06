cmake_minimum_required(VERSION 3.16)

project(Linux_socket)

add_executable(HelloWorld HelloWorld.cpp)

add_executable(TCP_echo TCP/echo_server.cpp)