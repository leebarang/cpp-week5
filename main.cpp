#include "console/console.h"
#include "game.h"
#include <cstdlib>
#include <ctime>

int main() {
  srand((unsigned int)time(nullptr));
  console::init(); // 초기화

  Game game; // 게임 생성

  while (!game.shouldExit()) { // 게임이 끝났는지 체크
    console::clear(); // 콘솔 클리어

    game.update(); // 게임 상황 업데이트
    game.draw(); // 게임 상황 콘솔 업로드

    // wait은 여기서만 호출되어야 한다.
    console::wait();
  }
}