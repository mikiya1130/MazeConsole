﻿#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MazeCommon.h"
#include "MazeGame.h"
#include "MazePlayer.h"

//迷路ゲーム
void MazeGame(){
    int goalCheck = 0;
    int mazeRow, mazeColumn;
    char buf[100];

    //プレイヤー
    MazeCell player;

    //迷路
    MazeBlock *maze;

    cls();

    //迷路サイズ入力
    printf("迷路の高さを入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", &mazeRow);
    while(mazeRow < 5 || mazeRow > 99 || mazeRow % 2 == 0){
        printf("\n入力値が不正です。迷路の高さを入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &mazeRow);
    }

    cls();

    printf("迷路の幅を入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", &mazeColumn);
    while(mazeColumn < 5 || mazeColumn > 99 || mazeColumn % 2 == 0){
        printf("\n入力値が不正です。迷路の幅を入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", &mazeColumn);
    }

    //配列の動的確保
    maze = (MazeBlock *)malloc(sizeof(MazeBlock) * mazeRow * mazeColumn);

    //if(maze == NULL){
    if(maze == NULL){
        cls();

        printf("メモリの確保に失敗しました。\n");
        printf("タイトルに戻ります。\n\n");
        system("pause");
        return;
    }

    //壁伸ばし法で迷路生成
    mazeCreate(maze, mazeRow, mazeColumn);

    //スタートとゴール位置
    maze[mazeColumn * 1 + 1].kind = START;
    maze[mazeColumn * (mazeRow - 2) + (mazeColumn - 2)].kind = GOAL;

    //プレイヤー初期化
    if(MazePlayerInit(&player.row, &player.column, maze, mazeRow, mazeColumn) == -1){
        //関数MazePlayerInitが-1を返すとき初期化に失敗している
        //よって、この時点でプログラムを終了し、迷路の表示は行わない
        return;
    }

    while(MazeGoalCheck(player.row, player.column, maze, mazeColumn) != 1){ //ゴールするまで移動を繰り返す
        //迷路表示
        MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);
        //プレイヤー移動
        MazePlayerMove(&player.row, &player.column, maze, mazeRow, mazeColumn);
    }

    //迷路最終結果表示
    MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);

    printf("\nゴール！\n\n");
    system("pause");

    free(maze);
}
//壁伸ばし法で迷路生成
void mazeCreate(MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;
    int index;
    int x, y;
    int dir;

    //壁生成開始候補座標リスト
    MazeCell *mazeStartCells;

    //壁生成開始候補座標リストの長さ
    int mazeCellLength = 0;

    //壁拡張可能方向リスト
    int directions[4];

    //配列の動的確保
    mazeStartCells = (MazeCell *)malloc(sizeof(MazeCell) * ((mazeRow - 3) / 2) * ((mazeColumn - 3) / 2));

    //マス目の初期化、壁伸ばし開始点の登録
    for(i = 0; i < mazeRow; i++){
        for(j = 0; j < mazeColumn; j++){
            //外周を道、他を道で初期化
            if(i == 0 || j == 0 || i == mazeRow - 1 || j == mazeColumn - 1){
                maze[mazeColumn * i + j].kind = WALL;
            }
            else{
                maze[mazeColumn * i + j].kind = PATH;

                //外周ではない偶数座標を壁伸ばし開始点にしておく
                if(i % 2 == 0 && j % 2 == 0){
                    mazeStartCells[mazeCellLength].row    = i;
                    mazeStartCells[mazeCellLength].column = j;
                    mazeCellLength++;
                }
            }
        }
    }

    //乱数の初期化
    srand((unsigned)time(NULL));

    //壁が拡張できなくなるまでループ
    while(mazeCellLength > 0){
        //ランダムに開始セルを取得
        index = rand() % mazeCellLength;
        x = mazeStartCells[index].row;
        y = mazeStartCells[index].column;

        //配列末尾の要素で上書きして、開始候補から削除
        mazeStartCells[index].row = mazeStartCells[mazeCellLength - 1].row;
        mazeStartCells[index].column = mazeStartCells[mazeCellLength - 1].column;

        //リストの長さ更新
        mazeCellLength--;

        //既に壁の場合は何もしない
        if(maze[mazeColumn * x + y].kind == WALL){
            continue;
        }

        //拡張開始地点を壁にする
        maze[mazeColumn * x + y].kind = EXWALL;

        //壁拡張可能方向リストの初期化
        for(i = 0; i < 4; i++){
            directions[i] = TRUE;
        }

        //壁を生成拡張
        while(  //全ての方向が探索済みになるまで(1つ以上TRUEの間ループ)
            directions[0] == TRUE
            || directions[1] == TRUE
            || directions[2] == TRUE
            || directions[3] == TRUE
        ){
            //拡張する方向
            dir = rand() % 4;

            //辿ってきた道 or 探索済みでないか判定
            if(directions[dir] == FALSE){
                continue;
            }

            switch(dir){
                //上に移動
                case UP:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * x + (y - 1)].kind == PATH
                        && maze[mazeColumn * x + (y - 2)].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y - 1)].kind = EXWALL;
                        maze[mazeColumn * x + (y - 2)].kind = EXWALL;

                        //座標更新
                        y -= 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //下は辿ってきた道
                        directions[DOWN] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y - 1)].kind == PATH
                        && maze[mazeColumn * x + (y - 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y - 1)].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //上は拡張不可
                        directions[UP] = FALSE;
                    }
                    break;

                //下に移動
                case DOWN:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * x + (y + 1)].kind == PATH
                        && maze[mazeColumn * x + (y + 2)].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y + 1)].kind = EXWALL;
                        maze[mazeColumn * x + (y + 2)].kind = EXWALL;

                        //座標更新
                        y += 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //上は辿ってきた道
                        directions[UP] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y + 1)].kind == PATH
                        && maze[mazeColumn * x + (y + 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y + 1)].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //下は拡張不可
                        directions[DOWN] = FALSE;
                    }
                    break;

                //左に移動
                case LEFT:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * (x - 1) + y].kind == PATH
                        && maze[mazeColumn * (x - 2) + y].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x - 1) + y].kind = EXWALL;
                        maze[mazeColumn * (x - 2) + y].kind = EXWALL;

                        //座標更新
                        x -= 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //右は辿ってきた道
                        directions[RIGHT] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x - 1) + y].kind == PATH
                        && maze[mazeColumn * (x - 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x - 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //左は拡張不可
                        directions[LEFT] = FALSE;
                    }
                    break;

                //右に移動
                case RIGHT:
                    if( //拡張可能なとき(2つ先まで道)
                        maze[mazeColumn * (x + 1) + y].kind == PATH
                        && maze[mazeColumn * (x + 2) + y].kind == PATH
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x + 1) + y].kind = EXWALL;
                        maze[mazeColumn * (x + 2) + y].kind = EXWALL;

                        //座標更新
                        x += 2;

                        //壁拡張可能方向リストの更新
                        for(i = 0; i < 4; i++){
                            directions[i] = TRUE;
                        }

                        //左は辿ってきた道
                        directions[LEFT] = FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x + 1) + y].kind == PATH
                        && maze[mazeColumn * (x + 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x + 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける)用
                        for(i = 0; i < 4; i++){
                            directions[i] = FALSE;
                        }
                    }
                    else{
                        //右は拡張不可
                        directions[RIGHT] = FALSE;
                    }
                    break;
            }
        }

        //拡張中だった壁を確定させる
        for(i = 0; i < mazeRow; i++){
            for(j = 0; j < mazeColumn; j++){
                if(maze[mazeColumn * i + j].kind == EXWALL){
                    maze[mazeColumn * i + j].kind = WALL;
                }
            }
        }
    }
}

//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;

    cls();

    for(i = 0; i < mazeRow; i++){  //行
        for(j = 0; j < mazeColumn; j++){   //列
            if(i == playerRow && j == playerColumn){    //プレイヤー位置
                printf("Ｐ");
            }
            else if(maze[mazeColumn * i + j].flag == FALSE){   //ブロックが判明していない場合
                printf("？");
            }
            else{
                switch(maze[mazeColumn * i + j].kind){
                    case WALL:
                        printf("□");    //壁
                        break;
                    case GOAL:
                        printf("Ｇ");    //ゴール
                        break;
                    default:
                        printf("　");    //その他(道、スタート)
                        break;
                }
            }
        }
        printf("\n");
    }
}

//ゴール判定
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock *maze, int mazeColumn){
    if(maze[mazeColumn * playerRow + playerColumn].kind == GOAL){ //プレイヤー位置がゴール地点に等しい
        return 1;
    }
    return 0;
}
