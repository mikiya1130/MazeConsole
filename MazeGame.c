#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "MazeCommon.h"
#include "MazeGame.h"
#include "MazePlayer.h"

//迷路ゲーム
void MazeGame(int menu){
    int i, j;
    int goalCheck = 0;
    int mazeRow, mazeColumn;

    //プレイヤー
    MazeCell player;

    //迷路
    MazeBlock *maze;

    //迷路サイズ定義
    switch(menu){
        case EASY:
        case EASY_H:
            mazeRow = 11;
            mazeColumn = 11;
            break;
        case NORMAL:
        case NORMAL_H:
            mazeRow = 21;
            mazeColumn = 21;
            break;
        case HARD:
        case HARD_H:
            mazeRow = 41;
            mazeColumn = 41;
            break;
        case CUSTOM:
        case CUSTOM_H:
            //迷路サイズ入力
            MazeSizeInput(&mazeRow, &mazeColumn);
            break;
    }

    //配列の動的確保
    maze = (MazeBlock *)malloc(sizeof(MazeBlock) * mazeRow * mazeColumn);

    //if(maze == NULL){
    if(maze == NULL){
        //コンソールクリア
        cls();

        printf("メモリの確保に失敗しました。\n");
        printf("タイトルに戻ります。\n\n");
        system("pause");
        return;
    }

    //壁伸ばし法で迷路生成
    MazeCreate(maze, mazeRow, mazeColumn);

    //迷路を隠すか否か
    switch(menu){
        //HIDEモードでないとき
        case EASY:
        case NORMAL:
        case HARD:
        case CUSTOM:
            for(i = 0; i < mazeRow; i++){
                for(j = 0; j < mazeColumn; j++){
                    maze[mazeColumn * i + j].flag = _TRUE;
                }
            }
            break;

        //HIDEモードのとき
        case EASY_H:
        case NORMAL_H:
        case HARD_H:
        case CUSTOM_H:
            for(i = 0; i < mazeRow; i++){
                for(j = 0; j < mazeColumn; j++){
                    if( //周囲とSTART,GOALは表示
                        i == 0 || i == mazeRow - 1
                        || j == 0 || j == mazeColumn - 1
                        || maze[mazeColumn * i + j].kind == START
                        || maze[mazeColumn * i + j].kind == GOAL
                    ){
                        maze[mazeColumn * i + j].flag = _TRUE;
                    }
                    else{
                        maze[mazeColumn * i + j].flag = _FALSE;
                    }
                }
            }
            break;
    }

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
    for(i = 0; i < mazeRow; i++){
        for(j = 0; j < mazeColumn; j++){
            maze[mazeColumn * i + j].flag = _TRUE;   //道を全て表示
        }
    }
    MazeDraw(player.row, player.column, maze, mazeRow, mazeColumn);

    printf("\nゴール！\n\n");
    system("pause");

    free(maze);
}

//迷路サイズ入力
void MazeSizeInput(int *mazeRow, int *mazeColumn){
    char buf[5];

    //コンソールクリア
    cls();

    //カーソル表示
    showCursor();

    //高さ入力
    printf("迷路の高さを入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", mazeRow);

    while(*mazeRow < 5 || *mazeRow > 99 || *mazeRow % 2 == 0){
        printf("\n入力値が不正です。迷路の高さを入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", mazeRow);
    }

    //コンソールクリア
    cls();

    //幅入力
    printf("迷路の幅を入力してください(5以上99以下の奇数)：");
    fgets(buf, sizeof(buf), stdin);
    sscanf_s(buf, "%d", mazeColumn);

    while(*mazeColumn < 5 || *mazeColumn > 99 || *mazeColumn % 2 == 0){
        printf("\n入力値が不正です。迷路の幅を入力してください(5以上99以下の奇数)：");
        fgets(buf, sizeof(buf), stdin);
        sscanf_s(buf, "%d", mazeColumn);
    }

    //カーソル非表示
    hideCursor();
}

//壁拡張可能方向リストの値を一括変更
void resetArray(enum MazeDirection arr[], int n, enum MazeFlag bl){
    for(int i = 0; i < n; i++){
        arr[i] = bl;
    }
}

//壁伸ばし法で迷路生成
void MazeCreate(MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;
    int index;
    int x, y;
    int dir;

    //壁生成開始候補座標リスト
    MazeCell *mazeStartCells = (MazeCell *)malloc(sizeof(MazeCell) * ((mazeRow - 3) / 2) * ((mazeColumn - 3) / 2));;

    //壁生成開始候補座標リストの長さ
    int mazeCellLength = 0;

    //壁拡張可能方向リスト
    enum MazeDirection directions[4];

    //マス目の初期化、壁伸ばし開始点の登録
    for(i = 0; i < mazeRow; i++){
        for(j = 0; j < mazeColumn; j++){
            //外周を壁、他を道で初期化
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

    //壁生成開始候補がなくなる(＝壁生成完了)までループ
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

        //拡張開始地点を壁(拡張中)にする
        maze[mazeColumn * x + y].kind = EXWALL;

        //壁拡張可能方向リストをTRUEで初期化
        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _TRUE);

        //壁を生成拡張
        while(  //全ての方向が探索済みになるまで(1つ以上TRUEの間ループ)
            directions[0] == _TRUE
            || directions[1] == _TRUE
            || directions[2] == _TRUE
            || directions[3] == _TRUE
        ){
            //拡張する方向
            dir = rand() % 4;

            //辿ってきた道 or 探索済みでないか判定
            if(directions[dir] == _FALSE){
                continue;
            }

            switch(dir){
                //上に拡張
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

                        //壁拡張可能方向リストをTRUEでリセット
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _TRUE);

                        //下は辿ってきた道
                        directions[DOWN] = _FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y - 1)].kind == PATH
                        && maze[mazeColumn * x + (y - 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y - 1)].kind = EXWALL;

                        //拡張終了(while文抜ける用にFALSE埋め)
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _FALSE);
                    }
                    else{
                        //上は拡張不可
                        directions[UP] = _FALSE;
                    }
                    break;

                //下に拡張
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

                        //壁拡張可能方向リストをTRUEでリセット
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _TRUE);

                        //上は辿ってきた道
                        directions[UP] = _FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * x + (y + 1)].kind == PATH
                        && maze[mazeColumn * x + (y + 2)].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * x + (y + 1)].kind = EXWALL;

                        //拡張終了(while文抜ける用にFALSE埋め)
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _FALSE);
                    }
                    else{
                        //下は拡張不可
                        directions[DOWN] = _FALSE;
                    }
                    break;

                //左に拡張
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

                        //壁拡張可能方向リストをTRUEでリセット
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _TRUE);

                        //右は辿ってきた道
                        directions[RIGHT] = _FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x - 1) + y].kind == PATH
                        && maze[mazeColumn * (x - 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x - 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける用にFALSE埋め)
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _FALSE);
                    }
                    else{
                        //左は拡張不可
                        directions[LEFT] = _FALSE;
                    }
                    break;

                //右に拡張
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

                        //壁拡張可能方向リストをTRUEでリセット
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _TRUE);

                        //左は辿ってきた道
                        directions[LEFT] = _FALSE;
                    }
                    else if( //拡張可能なとき(2つ先が壁)
                        maze[mazeColumn * (x + 1) + y].kind == PATH
                        && maze[mazeColumn * (x + 2) + y].kind == WALL
                    ){
                        //壁を拡張
                        maze[mazeColumn * (x + 1) + y].kind = EXWALL;

                        //拡張終了(while文抜ける用にFALSE埋め)
                        resetArray(directions, sizeof(directions) / sizeof(directions[0]), _FALSE);
                    }
                    else{
                        //右は拡張不可
                        directions[RIGHT] = _FALSE;
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

    //スタートとゴール位置
    maze[mazeColumn * 1 + 1].kind = START;
    maze[mazeColumn * (mazeRow - 2) + (mazeColumn - 2)].kind = GOAL;
}

//迷路表示
void MazeDraw(int playerRow, int playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn){
    int i, j;

    //コンソールクリア
    cls();

    for(i = 0; i < mazeRow; i++){  //行
        for(j = 0; j < mazeColumn; j++){   //列
            if(i == playerRow && j == playerColumn){    //プレイヤー位置
                printf("\033[36m"); //シアン
                printf("●");
                printf("\033[39m"); //元に戻す
            }
            else if(maze[mazeColumn * i + j].flag == _FALSE){   //ブロックが判明していない場合
                printf("？");
            }
            else{
                switch(maze[mazeColumn * i + j].kind){
                    case WALL:
                        printf("\033[47m"); //背景灰色
                        printf("　");    //壁
                        printf("\033[49m"); //背景戻す
                        break;
                    case GOAL:
                        printf("\033[36m"); //シアン
                        printf("○");    //ゴール
                        printf("\033[39m"); //元に戻す
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
