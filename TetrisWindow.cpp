//
// Created by Zahar.Koval on 21.08.22.
//

#include "TetrisWindow.h"
#include "Game.h"

#include <QKeyEvent>
#include <QTimer>
#include <QWindow>

#include <iostream>
#include <qwindowdefs.h>

TetrisWindow::TetrisWindow(QWidget *Parent)
    : QMainWindow(Parent), CurrentScene(GameScene{[this](unsigned Score) {
        CurrentScene.emplace<GameOverScene>(Score);
      }}),
      UpdateTimer(this) {
  setFixedSize(640, 480);

  connect(&UpdateTimer, &QTimer::timeout, this, &TetrisWindow::myUpdate);

  UpdateTimer.start(30);
}

void TetrisWindow::paintEvent(QPaintEvent *E) {
  std::visit([this](auto &&D) { D.draw(this); }, CurrentScene);
}

void TetrisWindow::keyPressEvent(QKeyEvent *E) {
  switch (E->key()) {
  case Qt::Key_Left:
    std::visit([](auto &&D) { D.onKey(KeyPressed::Left); }, CurrentScene);
    break;
  case Qt::Key_Right:
    std::visit([](auto &&D) { D.onKey(KeyPressed::Right); }, CurrentScene);
    break;
  case Qt::Key_Down:
    std::visit([](auto &&D) { D.onKey(KeyPressed::Down); }, CurrentScene);
    break;
  case Qt::Key_Up:
    std::visit([](auto &&D) { D.onKey(KeyPressed::Up); }, CurrentScene);
    break;
  }
}
