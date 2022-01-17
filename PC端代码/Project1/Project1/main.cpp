//
//  main.cpp
//  AIchess
//
//  Created by 蔡振 on 2021/12/15.
//

#include "port.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

WzSerialPort w;
void sendDemo(int buf)
{
    WzSerialPort w;
    char a[1];
    if (w.open("COM3", 9600, 0, 8, 1))
    {
        a[0] = buf;
            w.send(a , 1);
            w.close();
    }
    else
    {
        cout << "open serial port failed..." << endl;
    }
}

char* receive_Bytes() {
    WzSerialPort w;
    char buf[4];
    if (w.open("COM3", 9600, 0, 8, 1)) {
        memset(buf, 0, 4);
        //w.receive(buf, 4);
        while (!w.receive(buf, 4)) { cout << "no data, wait" << endl;  Sleep(1000); }
        w.close();
    }
    return buf;
}
int receive_oneByte()
{
    WzSerialPort w;
    char buf[1];
    if (w.open("COM3", 9600, 0, 8, 1))
    {
        memset(buf, 0, 1);
        //w.receive(buf, 1);
       while (!w.receive(buf, 1)) { cout << "no data, wait" << endl; Sleep(1000); };
            //cout << buf << endl;
         w.close();
    }
    return buf[0];
}

/* white:2 */
/* black:1 */
int chess[5][5] = { 0 };
int dir[4][2] = { {0,1},{0,-1},{1,0},{-1,0} };
int dir2[4][2] = { {0,2},{0,-2},{2,0},{-2,0} };
/* to count,also needed to own in other place,so don't send or receive */
int cntW = 5;
int cntB = 5;
/* input */
int sDataX_all_WHITE;
int sDataY_all_WHITE;
int dDataX_all_WHITE;
int dDataY_all_WHITE;
int AI_VS_Flag;         //AI:0、VS:1 默认为0
/* output */
int sDataX_all_BLACK;
int sDataY_all_BLACK;
int dDataX_all_BLACK;
int dDataY_all_BLACK;
int deleteX;
int deleteY;
int clearFlag = 0;
int winFlag;           //1:black win、2:white win


int isMoveFlag;
/* white:2 */
/* black:1 */
void chess_Init() {
    for (int j = 0; j < 5; j++) {
        chess[0][j] = 1;
        chess[4][j] = 2;
    }
}

void check_EquX(int tempY, int y, int x) {
    if (tempY > y) {
        if (tempY + 1 >= 0 && tempY + 1 < 5 && chess[x][tempY + 1] && chess[x][tempY + 1] != chess[x][y]) {
            clearFlag = chess[x][tempY + 1];
            deleteX = x;
            deleteY = tempY + 1;
        }
        else if (y - 1 >= 0 && y - 1 < 5 && chess[x][y - 1] && chess[x][y - 1] != chess[x][y]) {
            clearFlag = chess[x][y - 1];
            deleteX = x;
            deleteY = y - 1;
        }
    }
    else {
        if (tempY - 1 >= 0 && tempY - 1 < 5 && chess[x][tempY - 1] && chess[x][tempY - 1] != chess[x][y]) {
            clearFlag = chess[x][tempY - 1];
            deleteX = x;
            deleteY = tempY - 1;
        }
        else if (y + 1 >= 0 && y + 1 < 5 && chess[x][y + 1] && chess[x][y + 1] != chess[x][y]) {
            clearFlag = chess[x][y + 1];
            deleteX = x;
            deleteY = y + 1;
        }
    }
}

void check_EquY(int tempX, int x, int y) {
    if (tempX > x) {
        if (tempX + 1 >= 0 && tempX + 1 < 5 && chess[tempX + 1][y] && chess[tempX + 1][y] != chess[x][y]) {
            clearFlag = chess[tempX + 1][y];
            deleteX = tempX + 1;
            deleteY = y;
        }
        else if (x - 1 >= 0 && x - 1 < 5 && chess[x - 1][y] && chess[x - 1][y] != chess[x][y]) {
            clearFlag = chess[x - 1][y];
            deleteX = x - 1;
            deleteY = y;
        }
    }
    else {
        if (tempX - 1 >= 0 && tempX - 1 < 5 && chess[tempX - 1][y] && chess[tempX - 1][y] != chess[x][y]) {
            clearFlag = chess[tempX - 1][y];
            deleteX = tempX - 1;
            deleteY = y;
        }
        else if (x + 1 >= 0 && x + 1 < 5 && chess[x + 1][y] && chess[x + 1][y] != chess[x][y]) {
            clearFlag = chess[x + 1][y];
            deleteX = x + 1;
            deleteY = y;
        }
    }
}

/* 判断是否能吃、是否赢了 */
void check(int i, int j) {
    int tempX, tempY;
    for (int m = 0; m < 4; m++) {
        tempX = i + dir[m][0];
        tempY = j + dir[m][1];
        if (tempX >= 0 && tempX < 5 && tempY >= 0 && tempY < 5 && chess[tempX][tempY] == chess[i][j]) {
            if (tempX == i) {
                check_EquX(tempY, j, i);
            }
            else if (tempY == j) {
                check_EquY(tempX, i, j);
            }
        }
    }
}

int input_and_check(int sx, int sy, int dx, int dy, int w_or_b) {
    int re = 0;
    chess[sx][sy] = 0;
    chess[dx][dy] = w_or_b;
    check(dx, dy);
    if (clearFlag) {
        /* delete chess */
        //cout << "clearFlag = " << clearFlag << ",deleteX = " << deleteX << ",deleteY = " << deleteY << endl;
        sendDemo(clearFlag);
        sendDemo(deleteX);
        sendDemo(deleteY);
        
        chess[deleteX][deleteY] = 0;
        //cntW--;
        if (chess[dx][dy] == 1) {
            cntW--;
        }
        else if (chess[dx][dy] == 2) {
            cntB--;
        }
        if (cntW == 1) {
            winFlag = 1;
            re = 1;
        }
        if (cntB == 1) {
            winFlag = 2;
            re = 1;
        }
        //cout << "winFlag = " << winFlag << endl;
        sendDemo(winFlag);
        clearFlag = 0;
    }
    else {

        //cout << "clearFlag = " << clearFlag << endl;
        sendDemo(clearFlag);
    }
    return re;
}

void moveXY(int sx, int sy, int dx, int dy) {
    sDataX_all_WHITE = sx;
    sDataY_all_WHITE = sy;
    dDataX_all_WHITE = dx;
    dDataY_all_WHITE = dy;
    chess[sx][sy] = 0;
    chess[dx][dy] = 2;
    isMoveFlag = 1;
    cout << "move white = " << sx << " " << sy << " " << dx << " " << dy << endl;
    sendDemo(sx);
    sendDemo(sy);
    sendDemo(dx);
    sendDemo(dy);
}



/*
 *
 *  如果出现以下情况则依靠算法移进，否则先随机选择一个白子，用BFS方法四周寻找可以落脚点。
 *  1.
 *       O                  O *         *             *
 *         O *            O             O             O                 ...类似情况
 *                                    O                 O
 *  2.
 *         O                *   O           O       O   *
 *       *   O                O           O   *       O                 ...类似情况
 *
 */
void moveWhite() {
    int tempX;
    int tempY;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (chess[i][j] == 1) {      /* find black chess */
                for (int m = 0; m < 4; m++) {
                    if (isMoveFlag == 1) {
                        return;
                    }
                    tempX = i + dir[m][0];
                    tempY = j + dir[m][1];
                    if (tempX >= 0 && tempX < 5 && tempY >= 0 && tempY < 5 && chess[tempX][tempY] == 2) {
                        if (tempX == i) {
                            if (tempY > j) {
                                if (tempY + 1 >= 0 && tempY + 1 < 5 && chess[i][tempY + 1] == 0) {
                                    if (i + 1 >= 0 && i + 1 < 5 && chess[i + 1][tempY + 1] == 2 ) {
                                        moveXY(i + 1, tempY + 1, i, tempY + 1);
                                    }
                                    else if (i - 1 >= 0 && i - 1 < 5 && chess[i - 1][tempY + 1] == 2) {
                                        moveXY(i - 1, tempY + 1, i, tempY + 1);
                                    }
                                    else if (tempY + 2 >= 0 && tempY + 2 < 5 && chess[i][tempY + 2] == 2) {
                                        moveXY(i, tempY + 2, i, tempY + 1);
                                    }
                                }
                            }
                            else if (tempY < j) {
                                if (tempY - 1 >= 0 && tempY - 1 < 5 && chess[i][tempY - 1] == 0) {
                                    if (i + 1 >= 0 && i + 1 < 5 && chess[i + 1][tempY - 1] == 2) {
                                        moveXY(i + 1, tempY - 1, i, tempY - 1);
                                    }
                                    else if (i - 1 >= 0 && i - 1 < 5 && chess[i - 1][tempY - 1] == 2) {
                                        moveXY(i - 1, tempY - 1, i, tempY - 1);
                                    }
                                    else if (tempY - 2 >= 0 && tempY - 2 < 5 && chess[i][tempY - 2] == 2) {
                                        moveXY(i, tempY - 2, i, tempY - 1);
                                    }
                                }
                            }
                        }
                        else if (tempY == j) {
                            if (tempX < i) {
                                if (tempX - 1 >= 0 && tempX - 1 < 5 && chess[tempX - 1][j] == 0) {
                                    if (j - 1 >= 0 && j - 1 < 5 && chess[tempX - 1][j - 1] == 2) {
                                        moveXY(tempX - 1, j - 1, tempX - 1, j);
                                    }
                                    else if (j + 1 >= 0 && j + 1 < 5 && chess[tempX - 1][j + 1] == 2) {
                                        moveXY(tempX  - 1, j + 1, tempX  - 1, j);
                                    }
                                    else if (tempX - 2 >= 0 && tempX - 2 < 5 && chess[tempX - 2][j] == 2) {
                                        moveXY(tempX - 2, j, tempX - 1, j);
                                    }
                                }
                            }
                            else if (tempX > i) {
                                if (tempX  + 1>= 0 && tempX + 1 < 5 && chess[tempX + 1][j] == 0) {
                                    if (j - 1 >= 0 && j - 1 < 5 && chess[tempX + 1][j - 1] == 2) {
                                        moveXY(tempX  + 1, j - 1, tempX + 1, j);
                                    }
                                    else if (j + 1 >= 0 && j + 1 < 5 && chess[tempX + 1][j + 1] == 2) {
                                        moveXY(tempX + 1 , j + 1,  tempX + 1, j);
                                    }
                                    else if (tempX + 2 >= 0 && tempX + 2 < 5 && chess[tempX + 2][j] == 2) {
                                        moveXY(tempX + 2,  j, tempX + 1, j);
                                    }
                                }
                            }
                        }
                    }
                }
                if (isMoveFlag == 0) {
                    for (int m = 0; m < 4; m++) {
                        tempX = dir2[m][0] + i;
                        tempY = dir2[m][1] + j;
                        if (tempX >= 0 && tempX < 5 && tempY >= 0 && tempY < 5 && chess[tempX][tempY] == 2) {
                            if (tempX == i) {
                                if (!chess[i][(tempY + j) / 2]) {    //no chess
                                    if (i + 1 >= 0 && i + 1 < 5 && chess[i + 1][(tempY + j) / 2] == 2) {
                                        moveXY(i + 1, (tempY + j) / 2, i, (tempY + j) / 2);
                                    }
                                    else if (i - 1 >= 0 && i - 1 < 5 && chess[i - 1][tempY + j / 2] == 2) {
                                        moveXY(i - 1, (tempY + j) / 2, i, (tempY + j) / 2);
                                    }
                                }
                            }
                            else if (tempY == j) {
                                if (!chess[(i + tempX) / 2][j]) {
                                    if (j - 1 >= 0 && j - 1 < 5 && chess[(tempX + i) / 2][j - 1] == 2) {
                                        moveXY((tempX + i) / 2, j - 1, (tempX + i) / 2, j);
                                    }
                                    else if (j + 1 >= 0 && j + 1 < 5 && chess[(tempX + i) / 2][j + 1] == 2) {
                                        moveXY((tempX + i) / 2, j + 1, (tempX + i) / 2, j);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* have not moved */
    if (isMoveFlag == 0) {
        int i;
        int j;
        /* due to find the first is  to find black,so i don't change that */
        int isCheck[5][5] = { 0 };
        while(1) {
            /* begin: add random */
            i = rand() % 5;
            j = rand() % 5;
            while (isCheck[i][j] || chess[i][j] != 2) {
                i = rand() % 5;
                j = rand() % 5;
            }         
            for (int m = 0; m < 4; m++) {
                tempX = dir[m][0] + i;
                tempY = dir[m][1] + j;
                /* move to a blank position */
                if (tempX >= 0 && tempX < 5 && tempY >= 0 && tempY < 5 && !chess[tempX][tempY]) {
                    moveXY(i, j, tempX, tempY);
                    return;
                }
            }
            isCheck[i][j] = 1;

        }

    }
}


/* 为防止移动的原坐标和目的坐标相同，在touch后应判断原坐标和目的坐标是否相同，如果相同则忽0略本次操作，重新输入 -> 不在软件层做，在硬件层完成 */

int main() {
    int re = 0;
    chess_Init();
    //cin >> AI_VS_Flag;
    //sendDemo(1);
    AI_VS_Flag = receive_oneByte();
    //AI_VS_Flag = 0;
    if (AI_VS_Flag == 0) {           //AI
        srand(int(time(0)));
        while (!re) {
            //cin >> sDataX_all_BLACK >> sDataY_all_BLACK >> dDataX_all_BLACK >> dDataY_all_BLACK;
            char* buf = receive_Bytes();
            sDataX_all_BLACK = buf[0];
            sDataY_all_BLACK = buf[1];
            dDataX_all_BLACK = buf[2];
            dDataY_all_BLACK = buf[3];
            re = input_and_check(sDataX_all_BLACK, sDataY_all_BLACK, dDataX_all_BLACK, dDataY_all_BLACK, 1);
            if (re == 1) {
                break;
            }
            /* move white */
            isMoveFlag = 0;
            moveWhite();
            /* check */
            re = input_and_check(sDataX_all_WHITE, sDataY_all_WHITE, dDataX_all_WHITE, dDataY_all_WHITE, 2);
        }
    }
    else if (AI_VS_Flag == 1) {          //human
        while (!re) {
            //cin >> sDataX_all_BLACK >> sDataY_all_BLACK >> dDataX_all_BLACK >> dDataY_all_BLACK;
            char* buf = receive_Bytes();
            sDataX_all_BLACK = buf[0];
            sDataY_all_BLACK = buf[1];
            dDataX_all_BLACK = buf[2];
            dDataY_all_BLACK = buf[3];
            re = input_and_check(sDataX_all_BLACK, sDataY_all_BLACK, dDataX_all_BLACK, dDataY_all_BLACK, 1);
            if (re == 1) {
                break;
            }
            //cin >> sDataX_all_WHITE >> sDataY_all_WHITE >> dDataX_all_WHITE >> dDataY_all_WHITE;
            buf = receive_Bytes();
            sDataX_all_WHITE = buf[0];
            sDataY_all_WHITE = buf[1];
            dDataX_all_WHITE = buf[2];
            dDataY_all_WHITE = buf[3];
            re = input_and_check(sDataX_all_WHITE, sDataY_all_WHITE, dDataX_all_WHITE, dDataY_all_WHITE, 2);
        }
    }
}

