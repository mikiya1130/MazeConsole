#pragma once

//プロトタイプ宣言
int MazePlayerInit(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn);
int getArrowKey();
void MazePlayerMove(int *playerRow, int *playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn);
