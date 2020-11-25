cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 20)

project(JEngine)
add_subdirectory(engine)

project(shooteroo)
add_subdirectory(shooteroo)

set(resources shooteroo/resources)