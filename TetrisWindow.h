#pragma once

#include <QMainWindow>
#include <QTimer>
#include <variant>

#include "Game.h"

class TetrisWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit TetrisWindow(QWidget *Parent = nullptr);

  void paintEvent(QPaintEvent *) override;
  void keyPressEvent(QKeyEvent *) override;

public Q_SLOTS:
  void myUpdate() { update(); }

private:
  std::variant<GameOverScene, GameScene> CurrentScene;
  QTimer UpdateTimer;
};
