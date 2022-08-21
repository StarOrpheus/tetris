#pragma once

#include <QtWidgets/qmainwindow.h>
#include <cstdint>
#include <random>
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

struct TetrisWindow;

enum class KeyPressed {
  Left,
  Right,
  Down,
  Up
};

struct GameScene {
  constexpr static unsigned W = 10;
  constexpr static unsigned H = 20;

  using Position = unsigned;

  GameScene();

  void updateFlying();

  Position fromCartesian(unsigned I, unsigned J);

  std::pair<unsigned, unsigned> toCartesian(Position P);

  CellColor posColor(Position P);

  void draw(TetrisWindow* W);

  void onKey(KeyPressed);

  bool collide();
  bool flyingFit();

  unsigned Level = 1;
  std::vector<CellColor> Cells; //!< Current flying one is not included!
  TetrominoType CurrentFlyingType;
  uint8_t CurrentFlyingTurns; // number of clock-wise turns
  Position CurrentFlyingPos;
  uint64_t Score = 0;
};

struct GameOverScene {

  explicit GameOverScene(unsigned FinalScore) : FinalScore(FinalScore) {}

  void draw(TetrisWindow* W);

  void onKey(KeyPressed) {}

  unsigned FinalScore;
};
