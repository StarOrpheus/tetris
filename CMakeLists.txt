cmake_minimum_required(VERSION 3.21)
project(tetris)

set(CMAKE_CXX_STANDARD 20)

find_package(imgui CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable(main
        main.cpp
        GameScene.cpp GameScene.h
        GameOverScene.cpp GameOverScene.h)

target_link_libraries(main
        PRIVATE
        imgui::imgui
        fmt::fmt
)
