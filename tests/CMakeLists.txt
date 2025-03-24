# Find GTest package
find_package(GTest CONFIG REQUIRED)

# Add test executable
add_executable(tetris_tests
    GameSceneTests.cpp
    GameOverSceneTests.cpp
    ${CMAKE_SOURCE_DIR}/GameScene.cpp
    ${CMAKE_SOURCE_DIR}/GameOverScene.cpp
)

# Link against GTest and other dependencies
target_link_libraries(tetris_tests
    PRIVATE
    GTest::gtest
    GTest::gtest_main
    fmt::fmt
    imgui::imgui
)

# Add compile options (same as main target)
target_compile_options(tetris_tests PRIVATE -fno-rtti -fno-exceptions)

# Include the main project directory
target_include_directories(tetris_tests PRIVATE ${CMAKE_SOURCE_DIR})

# Add the test to CTest
include(GoogleTest)
gtest_discover_tests(tetris_tests)
