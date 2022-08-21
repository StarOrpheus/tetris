#include <QtWidgets/qapplication.h>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
#include <tuple>
#include <variant>
#include <vector>
#include <array>

#include "TetrisWindow.h"


int main(int Argc, char** Argv) {
  QApplication App(Argc, Argv);
  TetrisWindow W;
  W.show();
  return App.exec();
}
