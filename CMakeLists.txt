cmake_minimum_required(VERSION 3.21)
project(tetris)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(main
            MANUAL_FINALIZATION
            main.cpp
            TetrisWindow.cpp TetrisWindow.h
            Game.cpp Game.h
    )
else()
    add_executable(main
            main.cpp
            TetrisWindow.cpp TetrisWindow.h
            Game.cpp Game.h
    )
endif()

target_link_libraries(main
        PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)

