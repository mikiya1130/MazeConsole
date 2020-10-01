#pragma once

//プロトタイプ宣言
void MazeGame();
void MazeSizeInput(int *mazeRow, int *mazeColumn);
void resetArray(enum MazeDirection arr[], int n, enum MazeFlag bl);
void MazeCreate(MazeBlock *maze, int mazeRow, int mazeColumn);
void MazeDraw(int playerRow, int playerColumn, MazeBlock *maze, int mazeRow, int mazeColumn);
int MazeGoalCheck(int playerRow, int playerColumn, MazeBlock *maze, int mazeColumn);
