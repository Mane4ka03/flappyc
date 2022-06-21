cmake_minimum_required(VERSION 3.21)
project(cfgame)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 REQUIRED graphics network audio window system)


add_executable(cfgame main.cpp flappy.cpp)
target_link_libraries(cfgame sfml-graphics)



enable_testing()
add_subdirectory(doctest)

add_executable(tests flappy.cpp tests.cpp)
target_link_libraries(tests sfml-graphics doctest)

