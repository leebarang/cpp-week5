    #include "game.h"
    #include "tetromino.h"
    #include "console/console.h"
    #include <cstdlib>
    #include <ctime>

    // 테트로미노 모양
    Tetromino I("I", 4, "XXXXOOOOXXXXXXXX");
    Tetromino O("0", 2, "OOOO");
    Tetromino T("T", 3, "XOXOOOXXX");
    Tetromino S("S", 3, "XOOOOXXXX");
    Tetromino Z("Z", 3, "OOXXOOXXX");
    Tetromino J("J", 3, "OXXOOOXXX");
    Tetromino L("L", 3, "XXOOOOXXX");

    Tetromino currentTetromino = I; // 현재 테트리미노
    Tetromino nextTetromino = I; // 다음 테트리미노
    Tetromino holdingTetromino = I; // 홀딩 테트리미노
    Tetromino currentShadow = I; // 테트리미노랑 같은 쉐도우 만들기

    bool hold_empty = true; // 홀드가 비었는가
    bool is_hold = false; // 이번 턴에 홀드를 했는가

    void Game::printLeftLines() {
        std::string str_lines = std::to_string(left_lines);
        str_lines.append(" lines left");
        console::draw(0, BOARD_HEIGHT + 2, str_lines);
    }

    void Game::printWin() {
        console::clear();
        draw();
        console::draw(BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2, "You Win");
        console::draw(BOARD_WIDTH / 2 - 3, BOARD_HEIGHT / 2 + 1, getPlaytime(playtime));
        console::wait();
    }

    std::string Game::getPlaytime(int time) {
        int m, s, ss;
        ss = time % 100;
        time /= 100;
        s = time % 60;
        time /= 60;
        m = time;

        std::string str_m = std::to_string(m);
        std::string str_s = std::to_string(s);
        std::string str_ss = std::to_string(ss);

        if (m < 10) {
            str_m.insert(0, "0");
        }
        if (s < 10) {
            str_s.insert(0, "0");
        }
        if (ss < 10) {
            str_ss.insert(0, "0");
        }

        std::string str_playtime = str_m + ":" + str_s + "." + str_ss; 
        return str_playtime;
    }

    void Game::makeNewTetromino(int num) {
        int shape = rand()%7;
        switch (shape)
        {
        case 0:
            if (num == 0)
                currentTetromino = I;
            else
                nextTetromino = I;
            break;
        case 1:
            if (num == 0)
                currentTetromino = O;
            else
                nextTetromino = O;
            break;
        case 2:
            if (num == 0)
                currentTetromino = T;
            else
                nextTetromino = T;
            break;
        case 3:
            if (num == 0)
                currentTetromino = S;
            else
                nextTetromino = S;
            break;
        case 4:
            if (num == 0)
                currentTetromino = Z;
            else
                nextTetromino = Z;
            break;
        case 5:
            if (num == 0)
                currentTetromino = J;
            else
                nextTetromino = J;
        case 6:
            if (num == 0)
                currentTetromino = L;
            else
                nextTetromino = L;
            break;
        default:
            break;
        }
    }

    void Game::subLines() {
        left_lines--;
    }

    void Game::hardDrop() {
        y = sh;
    }

    void Game::hold() {
        if (hold_empty) {
            hold_empty = false;
            is_hold = true;
            holdingTetromino = *currentTetromino.original();
            currentTetromino = nextTetromino;
            makeNewTetromino(1);
        }
        if (!is_hold && !hold_empty) {
            is_hold = true;
            Tetromino temp = *currentTetromino.original();
            currentTetromino = holdingTetromino;
            holdingTetromino = temp;
        } 
    }

    void Game::keyInput() {
        if (console::key(console::K_LEFT)) {
            x--;
        }
        if (console::key(console::K_RIGHT)) {
            x++;
        }
        if (console::key(console::K_UP)) {
            hardDrop();
        }
        if (console::key(console::K_DOWN)) {
            y++;
        }
        if (console::key(console::K_SPACE)) {
            hold();
        }
        if (console::key(console::K_Z)) {
            currentTetromino = currentTetromino.rotatedCCW();
        }
        if (console::key(console::K_X)) {
            currentTetromino = currentTetromino.rotatedCW();
        }
    }

    void Game::clearLines() {
        
    }

    // 게임의 현재 상황을 업데이트 한다.
    void Game::update() {
        playtime++;
        timer++;
        keyInput();

        if (timer == DROP_DELAY) {
            timer = 0;
            y++;
            // if (!checkCollision(x, y + 1, currentTetromino)) { // 블록이 아래로 이동 가능한지 확인
            // y++;
            // } else { // 블록이 이동할 수 없을 때
            //     // 현재 위치에 블록을 고정
            //     for (int i = 0; i < currentTetromino.size(); i++) {
            //         for (int j = 0; j < currentTetromino.size(); j++) {
            //             if (currentTetromino.getShape()[i][j]) {
            //                 board_[x + j][y + i] = true;
            //             }
            //         }
            //     }
            //     clearLines(); // 가득 찬 줄이 있는지 확인하고 지우기
            //     x = 5; // 새로운 블록을 떨어뜨릴 위치
            //     y = 1;
            //     is_hold = false;
            //     currentTetromino = nextTetromino; // 다음 블록을 현재 블록으로 설정
            //     makeNewTetromino(1); // 새로운 다음 블록 생성
            // }
        } 
    }

    // 게임의 상황을 콘솔에 표시한다.
    void Game::draw() {
        console::drawBox(0, 0, BOARD_WIDTH + 1, BOARD_HEIGHT + 1); // 보드 1
        console::draw(BOARD_WIDTH / 2 - 3, BOARD_HEIGHT + 3, getPlaytime(playtime)); // 플레이 타임
        printLeftLines(); // 처리해야 할 남은 줄의 수

        console::drawBox(BOARD_WIDTH + 3, 0, BOARD_WIDTH + 8, 5); // 보드 2
        console::draw(BOARD_WIDTH + 4, 0, "Next");
        console::drawBox(BOARD_WIDTH + 9, 0, BOARD_WIDTH + 14, 5); // 보드 3
        console::draw(BOARD_WIDTH + 10, 0, "Hold");

        for (int i = 0; i < BOARD_WIDTH; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                if (board_[i][j] == true) {
                    console::draw(i + 1, j + 1, BLOCK_STRING);
                }
            }
        }

        currentTetromino.drawAt(SHADOW_STRING, x, sh); // 쉐도우
        currentTetromino.drawAt(BLOCK_STRING, x, y); // 테트로미노
        nextTetromino.drawAt(BLOCK_STRING, BOARD_WIDTH + 4, 1); // 다음 테트로미노
        if (!hold_empty) {
            holdingTetromino.drawAt(BLOCK_STRING, BOARD_WIDTH + 10, 1); // 홀딩 테트로미노
        }

    }

    bool Game::shouldExit() {
        if (console::key(console::K_ESC)) {
            return true;
        }
        if (left_lines <= 0) {
            printWin();
            return true;
        }
        return false;
    }

    Game::Game() {
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_HEIGHT; j++) {
                board_[i][j] = false;
            }
        }

        srand(time(NULL));

        timer = 0;
        left_lines = LINES;
        playtime = 0;
        x = 5;
        y = 1;
        bx = 0; // 블록의 가로 크기
        by = 0; // 블록의 세로 크기
        sh = 18; // 쉐도우의 높이

        currentTetromino = I;
        nextTetromino = I;
        makeNewTetromino(0);
        makeNewTetromino(1);
        holdingTetromino = I;
        currentShadow = currentTetromino;
    }