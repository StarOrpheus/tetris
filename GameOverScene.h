#pragma once

struct GameOverScene {

  constexpr explicit GameOverScene(unsigned FinalScore)
      : FinalScore(FinalScore) {}

  void draw();

  void onKey(int Key, int Action) {}

  unsigned FinalScore;
};
