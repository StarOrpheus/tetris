//
// Created by Zahar.Koval on 21.08.22.
//

#include "TetrisWindow.h"
#include "Game.h"

#include <QKeyEvent>

#include <iostream>
#include <qmainwindow.h>

TetrisWindow::TetrisWindow(QWidget *Parent)
    : QMainWindow(Parent), CurrentScene(GameScene{}), UpdateTimer(this) {
  setFixedSize(640, 480);

  QObject::connect(&UpdateTimer, SIGNAL(timeout()), this, SLOT(QMainWindow::requestUpdate()));

  UpdateTimer.start(50);
}

void TetrisWindow::paintEvent(QPaintEvent *E) {
  Frames++;
  std::cerr << Frames << std::endl;
  std::visit([this] (auto&& D) { D.draw(this); }, CurrentScene);
}

void TetrisWindow::keyPressEvent(QKeyEvent *E) {
  switch (E->key()) {
  case Qt::Key_Left:
    std::visit([] (auto&& D) { D.onKey(KeyPressed::Left); }, CurrentScene);
    break;
  case Qt::Key_Right:
    std::visit([] (auto&& D) { D.onKey(KeyPressed::Right); }, CurrentScene);
    break;
  case Qt::Key_Down:
    std::visit([] (auto&& D) { D.onKey(KeyPressed::Down); }, CurrentScene);
    break;
  case Qt::Key_Up:
    std::visit([] (auto&& D) { D.onKey(KeyPressed::Up); }, CurrentScene);
    break;
  }
}
