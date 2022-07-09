cmake_minimum_required(VERSION 3.20)
project(lab2_1 C)

set(CMAKE_C_STANDARD 99)

add_library(lab2_1 main.c matrix.c matrix.h ring.c ring.h streams.c streams.h)
