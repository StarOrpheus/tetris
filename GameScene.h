#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <array>

enum class CellColor : uint8_t {
  EmptyCell,

  // ####
  IColor,

  // #
  // ###
  JColor,

  //   #
  // ###
  LColor,

  // ##
  // ##
  OColor,

  //  ##
  // ##
  SColor,

  //  #
  // ###
  TColor,

  // ##
  //  ##
  ZColor
};

enum class TetrominoType : uint8_t { I, J, L, O, S, T, Z };

struct GameScene {
  constexpr static unsigned W = 10;
  constexpr static unsigned H = 20;
  constexpr static unsigned StartLevel = 0;

  using Position = unsigned;

  explicit GameScene(std::function<void(unsigned)> OnGameOver);

  void updateFlying() noexcept;

  [[nodiscard]] constexpr Position fromCartesian(unsigned I,
                                                 unsigned J) const noexcept;

  [[nodiscard]] constexpr std::pair<unsigned, unsigned>
  toCartesian(Position P) const noexcept;

  [[nodiscard]] constexpr CellColor posColor(Position P) const noexcept;

  void draw();

  void onKey(int Key, int Action);

  [[nodiscard]] constexpr bool collide(unsigned FlyingPos) const noexcept;

  [[nodiscard]] constexpr Position hardDropPos() const noexcept;

  constexpr void maybeLevelUp() noexcept;
  [[nodiscard]] constexpr unsigned linesToLevelUp() const noexcept;

private:
  unsigned Level = StartLevel;
  std::vector<CellColor> Cells; //!< Current flying one is not included!
  TetrominoType CurrentFlyingType;
  uint8_t CurrentFlyingTurns; //!< number of clock-wise turns
  Position CurrentFlyingPos;
  uint64_t Score = 0;
  uint32_t FullLinesOnCurLevel = 0;
  uint32_t Frame = 0;

  std::function<void(unsigned)> OnGameOver;
};
