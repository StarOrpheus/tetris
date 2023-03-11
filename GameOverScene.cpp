#include "GameOverScene.h"

#include "GraphicsIncludes.h"
#include <fmt/format.h>

void GameOverScene::draw() const {
  auto Text = fmt::format("Score: {}", FinalScore);

  auto WindowWidth = ImGui::GetWindowSize().x;
  auto WindowHeight = ImGui::GetWindowSize().y;
  auto TextWidth = ImGui::CalcTextSize(Text.data()).x;
  auto TextHeight = ImGui::CalcTextSize(Text.data()).y;

  ImGui::SetCursorPosX((WindowWidth - TextWidth) * 0.5f);
  ImGui::SetCursorPosY((WindowHeight - TextHeight) * 0.5f);
  ImGui::Text("%s", Text.data());
}
