# Tetris

A simple Tetris implementation using C++ and ImGui.

## Implementation Details

- Built with C++20
- Uses ImGui for UI rendering
- GLFW for window management and input handling
- OpenGL 3 for graphics rendering
- Scene-based architecture with game and game-over states
- Compiled with RTTI and exceptions disabled for better performance

## Main Features

- Classic Tetris gameplay with all 7 standard tetromino types (I, J, L, O, S, T, Z)
- Level progression system based on lines cleared
- Standard Tetris scoring system:
  - 1 line: 40 × (level + 1) points
  - 2 lines: 100 × (level + 1) points
  - 3 lines: 300 × (level + 1) points
  - 4 lines: 1200 × (level + 1) points
- Speed increases with each level
- Visual shadow showing where pieces will land
- Responsive grid that adapts to window size

## Controls

- Left/Right Arrow: Move tetromino horizontally
- Up Arrow: Rotate tetromino clockwise
- Down Arrow: Hard drop (instantly drop the tetromino)

## Known Limitations

- No way to restart the game after game over (must close and reopen)
- No pause functionality
- No high score tracking
- No sound effects
- No time tracking (though UI shows a placeholder for it)
- Limited customization options
- No menu system

## Dependencies

- fmt library
- ImGui with GLFW binding, OpenGL3 binding, and FreeType support

## Building

The project uses CMake for building and vcpkg for dependency management. Requires a C++20 compatible compiler.