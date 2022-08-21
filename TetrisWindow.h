#pragma once

#include <variant>
#include <QMainWindow>
#include <QTimer>

#include "Game.h"

class TetrisWindow : public QMainWindow {
  Q_OBJECT
public:

  explicit TetrisWindow(QWidget* Parent = nullptr);

  void paintEvent(QPaintEvent*) override;
  void keyPressEvent(QKeyEvent*) override;

private:
  std::variant<GameOverScene, GameScene> CurrentScene;
  QTimer UpdateTimer;
  size_t Frames = 0;
};
