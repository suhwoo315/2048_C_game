#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void resetScore();
void spawnBlock();
void mergeBlock();
void drawBoard();

unsigned score;
unsigned board[4][4];

int main(void){
    resetScore();
    
    spawnBlock();
    spawnBlock();

    return 0;
}

void resetScore(){
    score = 0;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            board[i][j] = 0;
        }
    }
}

void spawnBlock(){
    int i, j;
    srand(time(NULL));
    do
    {
        i = rand() % 4;
        j = rand() % 4;
    } while (board[i][j] != 0);
    board[i][j] = 2;
}

/*
for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
        printf("%d ", board[i][j]);
    }
    printf("\n");
}
*/

// best score => file I/O