#include "Game.h"
#include "TetrisWindow.h"

#include <cassert>
#include <tuple>

#include <QPoint>
#include <QBrush>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QPainterPath>


namespace {

CellColor matchColor(TetrominoType T) {
  switch (T) {
  case TetrominoType::I:
    return CellColor::IColor;
  case TetrominoType::J:
    return CellColor::JColor;
  case TetrominoType::L:
    return CellColor::LColor;
  case TetrominoType::O:
    return CellColor::OColor;
  case TetrominoType::S:
    return CellColor::SColor;
  case TetrominoType::T:
    return CellColor::TColor;
  case TetrominoType::Z:
    return CellColor::ZColor;
  }
  assert(false);
  return CellColor::EmptyCell;
}

std::array<std::pair<unsigned, unsigned>, 4>
getTetrominoCells(TetrominoType T, unsigned Rotations) {
  Rotations %= 4;
  switch (T) {
  case TetrominoType::I:
    if (Rotations & 1)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2}, std::pair{0, 3}};
    else
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{2, 0}, std::pair{3, 0}};
  case TetrominoType::J:
    if (Rotations == 0)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1}, std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 0}, std::pair{2, 0}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2}, std::pair{1, 2}};
    else
      return {std::pair{0, 1}, std::pair{1, 1}, std::pair{2, 1}, std::pair{2, 0}};
  case TetrominoType::L:
    if (Rotations == 0)
      return {std::pair{0, 2}, std::pair{1, 0}, std::pair{1, 1}, std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{2, 0}, std::pair{2, 1}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2}, std::pair{1, 0}};
    else
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 1}, std::pair{2, 1}};
  case TetrominoType::O:
    return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1}};
  case TetrominoType::S:
    if (Rotations & 1)
      return {std::pair{0, 1}, std::pair{0, 2}, std::pair{1, 0}, std::pair{1, 1}};
    else
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1}, std::pair{2, 1}};
  case TetrominoType::T:
    if (Rotations == 0)
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1}, std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1}, std::pair{2, 0}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2}, std::pair{1, 1}};
    else
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1}, std::pair{2, 1}};
  case TetrominoType::Z:
    if (Rotations & 1)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 1}, std::pair{1, 2}};
    else
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1}, std::pair{2, 0}};
  }
  assert(false);
  return {};
}

std::mt19937 &getRandomDevice() {
  static std::mt19937 D(std::random_device{}());
  return D;
}

QColor getColor(CellColor C) {
  switch (C) {
  case CellColor::EmptyCell:
    return {0, 0, 0};
  case CellColor::IColor:
    return {135, 206, 235};
  case CellColor::JColor:
    return {72, 180, 224};
  case CellColor::LColor:
    return {255, 153, 51};
  case CellColor::OColor:
    return {255, 255, 102};
  case CellColor::SColor:
    return {0, 114, 0};
  case CellColor::TColor:
    return {156, 81, 182};
  case CellColor::ZColor:
    return {120, 49, 20};
  }
  assert(false && "Unknown cell color");
  return {0, 0, 0};
}
} // namespace


GameScene::GameScene() : Cells(W * H, CellColor::EmptyCell) { updateFlying(); }

void GameScene::updateFlying() {
  std::uniform_int_distribution TypeDistribution(
      static_cast<unsigned>(TetrominoType::I),
      static_cast<unsigned>(TetrominoType::Z));

//  CurrentFlyingType = TetrominoType::I;
  CurrentFlyingType =
    static_cast<TetrominoType>(TypeDistribution(getRandomDevice()));
  CurrentFlyingTurns = 0;
  CurrentFlyingPos = fromCartesian(0, W / 2 - 1);
}

GameScene::Position GameScene::fromCartesian(unsigned int I, unsigned int J) {
  auto Result = I * W + J;
  assert(Result < W * H);
  return Result;
}

std::pair<unsigned, unsigned> GameScene::toCartesian(GameScene::Position P) {
  auto I = P / W;
  auto J = P % W;
  return {I, J};
}

CellColor GameScene::posColor(GameScene::Position P) {
  if (Cells[P] != CellColor::EmptyCell)
    return Cells[P];
  auto [X, Y] = toCartesian(P);
  auto [FlyingX, FlyingY] = toCartesian(CurrentFlyingPos);
  for (auto&& [dx, dy] : getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
    if (X == FlyingX + dx && Y == FlyingY + dy)
      return matchColor(CurrentFlyingType);
  }
  return CellColor::EmptyCell;
}

void GameScene::draw(TetrisWindow *Window) {
  QPainter P(Window);
  P.setRenderHint(QPainter::Antialiasing);

  unsigned CellSize = 20;

  std::pair<unsigned, unsigned> FrameSize(CellSize * W, CellSize * H);

  for (Position I = 0; I < Cells.size(); ++I) {
    auto Color = getColor(posColor(I));
    auto [X, Y] = toCartesian(I);
    QRect Cell((Y + 1) * CellSize, (X + 1) * CellSize, CellSize, CellSize);
    QPainterPath Path;
    Path.addRoundedRect(Cell, 2, 2, Qt::AbsoluteSize);
    QPen Pen(Qt::black, 2);
    P.setPen(Pen);
    P.fillPath(Path, Color);
    P.drawPath(Path);
  }
}

void GameScene::onKey(KeyPressed K) {
  assert(!collide());

  auto [X, Y] = toCartesian(CurrentFlyingPos);
  auto Old = CurrentFlyingPos;
  switch (K) {
  case KeyPressed::Left:
    if (Y > 0) {
      CurrentFlyingPos = fromCartesian(X, Y - 1);
      if (!flyingFit() || collide())
        CurrentFlyingPos = Old;
    }
    break;
  case KeyPressed::Right:
    if (Y < W - 1) {
      CurrentFlyingPos = fromCartesian(X, Y + 1);
      if (!flyingFit() || collide())
        CurrentFlyingPos = Old;
    }
    break;
  case KeyPressed::Down:
    // TODO
    break;
  case KeyPressed::Up:
    CurrentFlyingTurns++;
    break;
  }
}

bool GameScene::collide() {
  auto [FlyingX, FlyingY] = toCartesian(CurrentFlyingPos);
  for (auto &&[dx, dy] :
       getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
    auto P = fromCartesian(FlyingX + dx, FlyingY + dy);
    if (Cells[P] != CellColor::EmptyCell)
      return true;
  }
  return false;
}

bool GameScene::flyingFit() {
  auto [X, Y] = toCartesian(CurrentFlyingPos);
  for (auto &&[dx, dy] :
       getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
    if (X + dx >= H || Y + dy >= W)
      return false;
  }
  return true;
}

void GameOverScene::draw(TetrisWindow *W) {
 // TODO
}
