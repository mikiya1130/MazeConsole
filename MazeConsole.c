﻿#include <stdio.h>
#include <conio.h>
#include "MazeCommon.h"
#include "MazeGame.h"

//タイトル
int MazeTitle(){
    int menu = -1;

    //コンソールクリア
    cls();

    printf("＊＊＊迷路ゲーム＊＊＊\n\n");

    printf("メニュー\n");
    printf("%d:EASY     %d:EASY   (HIDE)\n", EASY, EASY_H);
    printf("%d:NORMAL   %d:NORMAL (HIDE)\n", NORMAL, NORMAL_H);
    printf("%d:HARD     %d:HARD   (HIDE)\n", HARD, HARD_H);
    printf("%d:CUSTOM   %d:CUSTOM (HIDE)\n", CUSTOM, CUSTOM_H);
    printf("%d:EXIT\n\n", EXIT);
    printf("数字で選択");

    while(menu < 0 || menu > EXIT){
        menu = _getch() - '0';
    }

    return menu;
}

int main(void){
    int menu;

    //カーソル非表示
    hideCursor();

    while(1){
        //メニュー選択
        menu = MazeTitle();

        if(menu == EXIT){   //EXITならwhileループを抜けて終了
            break;
        }

        //迷路ゲーム
        MazeGame(menu);
    }

    return 0;
}
