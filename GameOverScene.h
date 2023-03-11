#pragma once

struct GameOverScene {

  constexpr explicit GameOverScene(unsigned FinalScore) noexcept
      : FinalScore(FinalScore) {}

  void draw() const;

  constexpr void onKey(int Key, int Action) noexcept {}

private:
  unsigned FinalScore;
};
