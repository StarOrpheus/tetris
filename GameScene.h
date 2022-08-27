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

  GameScene(std::function<void(unsigned)> OnGameOver);

  void updateFlying();

  Position fromCartesian(unsigned I, unsigned J) const;

  std::pair<unsigned, unsigned> toCartesian(Position P) const;

  CellColor posColor(Position P);

  void draw();

  void onKey(int Key, int Action);

  bool collide(unsigned FlyingPos) const;

  Position hardDropPos() const;

  void maybeLevelUp();
  unsigned linesToLevelUp() const;

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
