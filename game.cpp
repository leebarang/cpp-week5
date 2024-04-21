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
    bool is_end = false; // 게임이 끝났는가
    std::string game_result = "";

    void Game::printLeftLines() {
        std::string str_lines = std::to_string(left_lines);
        str_lines.append(" lines left");
        console::draw(0, BOARD_HEIGHT + 2, str_lines);
    }

    void Game::printWin() {
        console::clear();
        game_result = "win";
        draw();
        is_end = true;
    }

    void Game::printLose() {
        console::clear();
        game_result = "lose";
        draw();
        is_end = true;
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
            break;
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

    bool Game::checkCollision(int newX, int newY, Tetromino tetromino) {
        // 테트로미노의 모든 블록을 순회하며 충돌을 검사합니다.
        for (int i = 0; i < tetromino.size(); i++) {
            for (int j = 0; j < tetromino.size(); j++) {
                if (tetromino.check(i, j)) {
                    int boardX = newX + j - 1;
                    int boardY = newY + i - 1;

                    // 게임 보드 밖으로 벗어나는 경우 충돌로 처리합니다.
                    if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                        return true;
                    }

                    // 다른 블록과 충돌하는지 확인합니다.
                    if (boardY >= 0 && board_[boardX][boardY]) {
                        return true;
                    }
                }
            }
        }
    // 충돌이 없는 경우
    return false;
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
            if (!checkCollision(x - 1, y, currentTetromino)) {
                x--;
            }
        }
        if (console::key(console::K_RIGHT)) {
            if (!checkCollision(x + 1, y, currentTetromino)) {
                x++;
            }
        }
        if (console::key(console::K_UP)) {
            hardDrop();
        }
        if (console::key(console::K_DOWN)) {
            if (!checkCollision(x, y + 1, currentTetromino)) {
                y++;
            }
        }
        if (console::key(console::K_SPACE)) {
            hold();
        }
        if (console::key(console::K_Z)) {
            currentTetromino = currentTetromino.rotatedCCW();
            if (checkCollision(x, y, currentTetromino)) {
                currentTetromino = currentTetromino.rotatedCW(); // 회전이 불가능한 경우 이전 상태로 복구
            }
        }
        if (console::key(console::K_X)) {
            currentTetromino = currentTetromino.rotatedCW();
            if (checkCollision(x, y, currentTetromino)) {
                currentTetromino = currentTetromino.rotatedCCW(); // 회전이 불가능한 경우 이전 상태로 복구
            }
        }
    }

    void Game::clearLines() {
        for (int j = BOARD_HEIGHT - 1; j >= 0; j--) { // 보드의 가장 아래부터 시작하여 위로 올라감
            bool lineFull = true; // 해당 줄이 가득 찼는지 여부를 나타내는 플래그

            // 해당 줄의 모든 셀이 블록으로 채워져 있는지 확인
            for (int i = 0; i < BOARD_WIDTH; i++) {
                if (!board_[i][j]) {
                    lineFull = false;
                    break;
                }
            }

            if (lineFull) { // 해당 줄이 가득 찼다면
                // 해당 줄을 지우고 위에 있는 줄들을 한 칸씩 아래로 내림
                for (int k = j; k > 0; k--) {
                    for (int i = 0; i < BOARD_WIDTH; i++) {
                        board_[i][k] = board_[i][k - 1];
                    }
                }
                // 맨 위의 줄은 모두 비워줌
                for (int i = 0; i < BOARD_WIDTH; i++) {
                    board_[i][0] = false;
                }
                // 지워진 줄의 수를 감소시키고 다음 줄부터 다시 확인
                left_lines--;
                j++; // 이 부분이 없으면 한 줄을 지운 후 그 다음 줄을 제대로 확인하지 못함
            }
        }
    }

    // 블록이 도착했다.
    void Game::blockArrived() {
        // 현재 위치에 블록을 고정
        for (int i = 0; i < currentTetromino.size(); i++) {
            for (int j = 0; j < currentTetromino.size(); j++) {
                if (currentTetromino.check(i,j)) {
                    board_[x + j - 1][y + i - 1] = true;
                }
            }
        }
        clearLines(); // 가득 찬 줄이 있는지 확인하고 지우기
        x = 5; // 새로운 블록을 떨어뜨릴 위치
        y = 1;
        is_hold = false;
        currentTetromino = nextTetromino; // 다음 블록을 현재 블록으로 설정
        makeNewTetromino(1); // 새로운 다음 블록 생성
    }

    // 게임의 현재 상황을 업데이트 한다.
    void Game::update() {
        playtime++;
        timer++;
        keyInput();

        // 만약 새로운 테트로미노가 생성되어야 하는데 이미 블록이 있으면 패배
        if (checkCollision(x, y, currentTetromino)) {
            printLose();
        }

        int shadowY = y;
        while (!checkCollision(x, shadowY + 1, currentTetromino)) {
            shadowY++;
        }
        // 가장 아래로 내려갈 수 있는 위치를 shadow의 위치로 설정
        sh = shadowY;

        if (timer == DROP_DELAY) {
            timer = 0; // 타이머 초기화
            if (y == sh) { // 블록이 도착 했을 때
                blockArrived();
            } else { // 블록이 아직 도착하지 않았을 때 블록이 이동한다.
                y++;
            }
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

        if (game_result == "win") {
            console::draw(BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2, "You Win");
            console::draw(BOARD_WIDTH / 2 - 3, BOARD_HEIGHT / 2 + 1, getPlaytime(playtime));
        }
        if (game_result == "lose") {
            console::draw(BOARD_WIDTH / 2 - 2, BOARD_HEIGHT / 2, "You Lose");
            console::draw(BOARD_WIDTH / 2 - 3, BOARD_HEIGHT / 2 + 1, getPlaytime(playtime));
        }

    }

    bool Game::shouldExit() {
        if (console::key(console::K_ESC)) {
            return true;
        }
        if (is_end) {
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