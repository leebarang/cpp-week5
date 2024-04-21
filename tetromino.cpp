#include "tetromino.h"
#include "console/console.h"

Tetromino Tetromino::I("I", 4, "XXXXOOOOXXXXXXXX");
Tetromino Tetromino::O("0", 2, "OOOO");
Tetromino Tetromino::T("T", 3, "XOXOOOXXX");
Tetromino Tetromino::S("S", 3, "XOOOOXXXX");
Tetromino Tetromino::Z("Z", 3, "OOXXOOXXX");
Tetromino Tetromino::J("J", 3, "OXXOOOXXX");
Tetromino Tetromino::L("L", 3, "XXOOOOXXX");


Tetromino::Tetromino(std::string name, int size, std::string shape) {
    name_ = name;
    size_ = size;
    left_hit, right_hit, down_hit = 0;
    // 초기화
    for (int i = 0; i < MAX_SIZE; i++) {
        for (int j = 0; j < MAX_SIZE; j++) {
            shape_[i][j] = false;
        }
    }
    int k = 0;
    
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            if (shape[k++] == 'O') { // shape 배열의 O 부분을 찾아 테트로미노의 블록을 만듦
                if (left_hit == 0) {
                    left_hit = j;
                    right_hit = j;
                }
                if (down_hit == 0) {
                    down_hit = i;
                }
                else {
                    left_hit = std::min(left_hit,j);
                    right_hit = std::max(right_hit,j);
                    down_hit = std::max(down_hit,i);
                }
                    
                shape_[i][j] = true;
            }
        }
    }

    if (name == "I") {
        original_ = &I;
    }
    else if (name == "O") {
        original_ = &O;
    }
    else if (name == "T") {
        original_ = &T;
    }
    else if (name == "S") {
        original_ = &S;
    }
    else if (name == "Z") {
        original_ = &Z;
    }
    else if (name == "J") {
        original_ = &J;
    }
    else if (name == "L") {
        original_ = &L;
    }

}

//key_X 시계 방향
Tetromino Tetromino::rotatedCW() {
    std::string rotatedShape = "";
    
    for (int y = size_ - 1; y >= 0; y--) {
        for (int x = 0; x < size_; x++) {
            if (check(x, y)) {
                rotatedShape.append("O");
            }
            else {
                rotatedShape.append("X");
            }
        }
    }
   
   return Tetromino(name_, size_, rotatedShape);
}

//key_Z 반시계 방향
Tetromino Tetromino::rotatedCCW() {
    std::string rotatedShape = "";
    
    for (int y = 0; y < size_; y++) {
        for (int x = size_ - 1; x >= 0; x--) {
            if (check(x, y)) {
                rotatedShape.append("O");
            }
            else {
                rotatedShape.append("X");
            }
        }
    }
   
   return Tetromino(name_, size_, rotatedShape);
}

// 테트로미노 위치를 콘솔에 나타냄
void Tetromino::drawAt(std::string s, int x, int y) {
    for (int i = 0; i < size_; i++) {
        for (int j = 0; j < size_; j++) {
            if (shape_[i][j]) {
                console::draw(x + j, y + i, s);
            }
        }
    }
}