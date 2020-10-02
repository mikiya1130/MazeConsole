#pragma once

//メニュー
enum MazeMenu {EASY, EASY_H, NORMAL, NORMAL_H, HARD, HARD_H, CUSTOM, CUSTOM_H, EXIT};

//マス目(プレイヤー座標、壁生成開始座標)
typedef struct {
    int row;
    int column;
} MazeCell;

//迷路の一ブロック
enum MazeKind {PATH, WALL, EXWALL, START, GOAL};    //ブロックの種類(道、壁、スタート、ゴール)
enum MazeFlag {_FALSE, _TRUE};                //ブロックが判明しているかどうか

typedef struct {
    enum MazeKind kind; //種類(道、壁、スタート、ゴール)
    enum MazeFlag flag; //ブロックが判明しているかどうか
} MazeBlock;

//方向
enum MazeDirection {UP, DOWN, LEFT, RIGHT, Invalid};

//プロトタイプ宣言
void cls();
void hideCursor();
void showCursor();
