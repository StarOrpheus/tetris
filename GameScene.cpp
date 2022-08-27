#include "GameScene.h"

#include "GraphicsIncludes.h"

#include <cassert>
#include <numeric>
#include <random>
#include <tuple>

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
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2},
              std::pair{0, 3}};
    else
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{2, 0},
              std::pair{3, 0}};
  case TetrominoType::J:
    if (Rotations == 0)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 0},
              std::pair{2, 0}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2},
              std::pair{1, 2}};
    else
      return {std::pair{0, 1}, std::pair{1, 1}, std::pair{2, 1},
              std::pair{2, 0}};
  case TetrominoType::L:
    if (Rotations == 0)
      return {std::pair{0, 2}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{2, 0},
              std::pair{2, 1}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2},
              std::pair{1, 0}};
    else
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 1},
              std::pair{2, 1}};
  case TetrominoType::O:
    return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1}};
  case TetrominoType::S:
    if (Rotations & 1)
      return {std::pair{0, 1}, std::pair{0, 2}, std::pair{1, 0},
              std::pair{1, 1}};
    else
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{2, 1}};
  case TetrominoType::T:
    if (Rotations == 0)
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{1, 2}};
    else if (Rotations == 1)
      return {std::pair{0, 0}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{2, 0}};
    else if (Rotations == 2)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{0, 2},
              std::pair{1, 1}};
    else
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{2, 1}};
  case TetrominoType::Z:
    if (Rotations & 1)
      return {std::pair{0, 0}, std::pair{0, 1}, std::pair{1, 1},
              std::pair{1, 2}};
    else
      return {std::pair{0, 1}, std::pair{1, 0}, std::pair{1, 1},
              std::pair{2, 0}};
  }
  assert(false);
  return {};
}

std::mt19937 &getRandomDevice() {
  static std::mt19937 D(std::random_device{}());
  return D;
}

ImColor getColor(CellColor C) {
  switch (C) {
  case CellColor::EmptyCell:
    return {255, 255, 255};
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

GameScene::GameScene(std::function<void(unsigned)> OnGameOver)
    : Cells(W * H, CellColor::EmptyCell), OnGameOver(std::move(OnGameOver)) {
  updateFlying();
}

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

GameScene::Position GameScene::fromCartesian(unsigned int I,
                                             unsigned int J) const {
  auto Result = I * W + J;
  assert(Result < W * H);
  return Result;
}

std::pair<unsigned, unsigned>
GameScene::toCartesian(GameScene::Position P) const {
  auto I = P / W;
  auto J = P % W;
  return {I, J};
}

CellColor GameScene::posColor(GameScene::Position P) {
  if (Cells[P] != CellColor::EmptyCell)
    return Cells[P];
  auto [X, Y] = toCartesian(P);
  auto [FlyingX, FlyingY] = toCartesian(CurrentFlyingPos);
  for (auto &&[dx, dy] :
       getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
    if (X == FlyingX + dx && Y == FlyingY + dy)
      return matchColor(CurrentFlyingType);
  }
  return CellColor::EmptyCell;
}

void GameScene::draw() {
  Frame++;

  // TODO: level-dependent speed
  // TODO: instant falling on Down key
  if (Frame % 30 == 0) {
    auto Old = CurrentFlyingPos;
    CurrentFlyingPos += W; // ++x
    if (collide(CurrentFlyingPos)) {
      CurrentFlyingPos = Old;
      auto [X, Y] = toCartesian(CurrentFlyingPos);
      for (auto &&[dx, dy] :
           getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
        Cells[fromCartesian(X + dx, Y + dy)] = matchColor(CurrentFlyingType);
      }

      std::vector<bool> Filled(H, true);
      for (size_t I = 0; I < H; ++I) {
        for (size_t J = 0; J < W; ++J) {
          if (Cells[fromCartesian(I, J)] == CellColor::EmptyCell) {
            Filled[I] = false;
            break;
          }
        }
      }
      std::vector<CellColor> New(Cells.size(), CellColor::EmptyCell);
      auto NextLine = H - 1;
      for (size_t I = H; I > 0; I--) {
        if (Filled[I - 1])
          continue;
        for (size_t J = 0; J < W; ++J)
          New[fromCartesian(NextLine, J)] = Cells[fromCartesian(I - 1, J)];
        NextLine--;
      }

      std::swap(New, Cells);

      unsigned FullLines = std::accumulate(Filled.begin(), Filled.end(), 0);
      if (FullLines) {
        FullLinesOnCurLevel += FullLines;
        if (FullLines == 1)
          Score += 40 * (Level + 1);
        else if (FullLines == 2)
          Score += 100 * (Level + 1);
        else if (FullLines == 3)
          Score += 300 * (Level + 1);
        else if (FullLines == 4)
          Score += 1200 * (Level + 1);
        else {
          assert(false && "Somehow more than 4 full lines detected!!");
        }
      }

      updateFlying();
      if (collide(CurrentFlyingPos))
        return OnGameOver(Score);
    }
  }
  assert(!collide(CurrentFlyingPos) &&
         "All collisions expected to be fixed during physics stage");

  {
    ImGui::BeginChild("left pane", ImVec2(150, 0), /*border=*/true);
    ImGui::Text("Score: %lu", Score);
    ImGui::Text("Level: %d", Level);
    ImGui::Text("Lines: %d/%d", FullLinesOnCurLevel, linesToLevelUp());
    ImGui::Text("Time: %d", 0); // TODO: add time tracker
    ImGui::EndChild();
  }

  ImGui::SameLine();

  {
    if (ImGui::BeginChild("right pane", ImVec2(0, 0), /*border=*/true)) {
      const auto WinSize = ImGui::GetWindowSize();
      constexpr float Spacing = 1.0f;

      auto StartPos = ImGui::GetCursorScreenPos();
      const auto BlockSize =
          std::min(WinSize.x / 10, WinSize.y / 20) - Spacing - 1;
      const auto BlockSizeWithSpacing = BlockSize + Spacing;

      const auto TopBottomMargin = (WinSize.y - BlockSizeWithSpacing * 20) / 2;
      const auto LeftRightMargin = (WinSize.x - BlockSizeWithSpacing * 10) / 2;
      const auto RowSize = BlockSizeWithSpacing;

      StartPos.x += LeftRightMargin;
      StartPos.y += TopBottomMargin;

      const float Rounding = BlockSize / 5.0f;

      // Как говорил мне мой дед:
      // - Я твой дед.
      float X = StartPos.x;
      float Y = StartPos.y;
      ImDrawList *DrawList = ImGui::GetWindowDrawList();
      auto HardDropPos = hardDropPos();

      for (size_t I = 0; I < H; ++I) {
        X = StartPos.x;
        if (I != 0)
          Y += BlockSize + Spacing;

        char IdBuf[20];
        sprintf(IdBuf, "right pane##%zu", I);

        for (size_t J = 0; J < W; ++J) {
          CellColor CColor = posColor(fromCartesian(I, J));
          bool OnShadowCell = false;
          auto [ShadowX, ShadowY] = toCartesian(HardDropPos);
          for (auto &&[dx, dy] :
               getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
            if (I == ShadowX + dx && J == ShadowY + dy) {
              OnShadowCell = true;
              break;
            }
          }

          auto Color = getColor(CColor);

          if (CColor != CellColor::EmptyCell) {
            DrawList->AddRectFilled(ImVec2(X, Y),
                                    ImVec2(X + BlockSize, Y + BlockSize), Color,
                                    Rounding, ImDrawFlags_None);
          } else if (OnShadowCell) {
            CellColor C = matchColor(CurrentFlyingType);
            Color = getColor(C);
            DrawList->AddRect(ImVec2(X, Y),
                              ImVec2(X + BlockSize, Y + BlockSize), Color,
                              Rounding, ImDrawFlags_None, 3.0f);
          } else {
            assert(CColor == CellColor::EmptyCell);
            DrawList->AddRect(ImVec2(X, Y),
                              ImVec2(X + BlockSize, Y + BlockSize), Color,
                              Rounding, ImDrawFlags_None, 1.0f);
          }

          X += BlockSize + Spacing;
        }
      }
    }
    ImGui::EndChild();
  }
}

void GameScene::onKey(int Key, int Action) {
  assert(!collide(CurrentFlyingPos) &&
         "No collision before key event expected");

  auto [X, Y] = toCartesian(CurrentFlyingPos);
  auto Old = CurrentFlyingPos;

  if (Action != GLFW_PRESS)
    return;

  if (Key == GLFW_KEY_LEFT) {
    if (Y > 0) {
      CurrentFlyingPos = fromCartesian(X, Y - 1);
      if (collide(CurrentFlyingPos))
        CurrentFlyingPos = Old;
    }
  } else if (Key == GLFW_KEY_RIGHT) {
    if (Y < W - 1) {
      CurrentFlyingPos = fromCartesian(X, Y + 1);
      if (collide(CurrentFlyingPos))
        CurrentFlyingPos = Old;
    }
  } else if (Key == GLFW_KEY_UP) {
    CurrentFlyingTurns++;
    if (collide(CurrentFlyingPos))
      CurrentFlyingTurns--;
  } else if (Key == GLFW_KEY_DOWN) {
    CurrentFlyingPos = hardDropPos();
  }

  assert(!collide(CurrentFlyingPos) && "Pressed key cause collision");
}

bool GameScene::collide(unsigned FlyingPos) const {
  auto [FlyingX, FlyingY] = toCartesian(FlyingPos);
  for (auto &&[dx, dy] :
       getTetrominoCells(CurrentFlyingType, CurrentFlyingTurns)) {
    if (FlyingX + dx >= H || FlyingY + dy >= W)
      return true;
    auto P = fromCartesian(FlyingX + dx, FlyingY + dy);
    if (Cells[P] != CellColor::EmptyCell)
      return true;
  }
  return false;
}

void GameScene::maybeLevelUp() {
  if (FullLinesOnCurLevel > linesToLevelUp()) {
    Level++;
    FullLinesOnCurLevel = 0;
  }
}

unsigned GameScene::linesToLevelUp() const {
  return std::min(StartLevel * 10 + 10,
                  unsigned(std::max(100, int(StartLevel) * 10 - 50)));
}

GameScene::Position GameScene::hardDropPos() const {
  auto FlyingPos = CurrentFlyingPos;
  assert(!collide(FlyingPos));
  while (true) {
    auto [X, Y] = toCartesian(FlyingPos);
    Position NextPos = fromCartesian(X + 1, Y);
    if (collide(NextPos))
      return FlyingPos;
    FlyingPos = NextPos;
  }
}
