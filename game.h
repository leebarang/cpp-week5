
#ifndef GAME_H
#define GAME_H

#include "tetromino.h"

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define LINES 40

#define DROP_DELAY 60

class Game {
private:
  // 게임 판을 나타내는 배열
  // board[x][y]가 true 인 경우 x, y 위치에 블록이 존재하는 것을 의미한다
  bool board_[BOARD_WIDTH][BOARD_HEIGHT];
  int timer;
  int left_lines;
  int playtime;
  int x, y, bx, by, sh;

  // 처리해야 할 남은 줄의 개수를 화면에 출력한다.
  void printLeftLines();

  // 승리 화면을 출력한다.
  void printWin();

  // 현재까지의 플레이 타임을 반환한다.
  std::string getPlaytime(int time);

  // 새로운 테트로미노를 만든다. (0 : current, 1 : next)
  void makeNewTetromino(int num);

  // 테스트용 기능
  void subLines();

  // 키보드 입력을 처리한다.
  void keyInput();

  // Hard Drop
  void hardDrop();

  // Holding
  void hold();

  // 블록이 움직일 수 있는가
  bool checkCollision(int x, int y, const Tetromino& tetromino) const;

  // 줄 지우기
  void clearLines();

public:
  // 게임의 한 프레임을 처리한다.
  void update();

  // 게임 화면을 그린다.
  void draw();

  // 게임 루프가 종료되어야 하는지 여부를 반환한다.
  bool shouldExit();

  Game();
};
#endif