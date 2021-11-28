#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

void setBufferedInput(int);
void titleScreen();
void resetGame();
void spawnBlock();
void drawBoard();
void moveUp();
void moveLeft();
int moveDown();
void moveRight();

unsigned score;
unsigned board[4][4];

int main(void){
    setBufferedInput(0);
    titleScreen();

    resetGame();
    spawnBlock();
    spawnBlock();
    drawBoard();

    while(1){
        char c = getchar();
        switch (c)
        {
        case 'w':
        case 65:
            moveUp();
            spawnBlock();
            drawBoard();
            break;
        case 'a':
        case 68:
            moveLeft();
            spawnBlock();
            drawBoard();
            break;
        case 's':
        case 66:
            if(moveDown()) spawnBlock();
            drawBoard();
            break;
        case 'd':
        case 67:
            moveRight();
            spawnBlock();
            drawBoard();
            break;
        case 'q':
            printf("quit\n"); // implement this
            break;
        default:
            break;
        }
    }

    return 0;
}

void titleScreen(){
    char c = 'S';
    printf("Use arrow keys or wasd\n");
    printf("Please turn off caps lock first\n");
    printf("Press lowercase 's' to start\n");
    while(c != 's'){
        c = getchar();
    }
}

void setBufferedInput(int enable) {
	static int enabled = 1;
	static struct termios old;
	struct termios new;
	if (enable && !enabled) {
		tcsetattr(STDIN_FILENO,TCSANOW,&old);
		enabled = 1;
	} else if (!enable && enabled) {
		tcgetattr(STDIN_FILENO,&new);
		old = new;
		new.c_lflag &=(~ICANON & ~ECHO);
		tcsetattr(STDIN_FILENO,TCSANOW,&new);
		enabled = 0;
	}
}

void resetGame(){
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

void drawBoard(){
    // temporary
    printf("\nscore : %d\n", score);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void moveUp(){}

void moveLeft(){}

/*
void moveDown(){
    for(int j = 0; j < 4; j++){
        int num1 = 0, num2 = 0;
        int index = 3;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] == 0) continue;
            if (num1 == 0) {
                num1 = board[i][j];
            }
            else if (num2 == 0) {
                num2 = board[i][j];
                if (num1 == num2){
                    board[index][j] = num1 << 1;
                    index--;
                    score += num1 << 1;
                    num1 = num2 = 0;
                }
                else {
                    board[index][j] = num1;
                    index--;
                    num1 = num2;
                    num2 = 0;
                }
            }
        }
        if (num1 != 0) {
            board[index][j] = num1;
            index--;
        }
        for(int i = 0; i <= index; i++) board[i][j] = 0;
    }
}
*/

/*
void moveDown(){
    for(int j = 0; j < 4; j++){
        int num = 0;
        int index = 3;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] == 0) continue;
            if (num == 0) {
                num = board[i][j];
                board[i][j] = 0;
                continue;
            }
            if (num == board[i][j]) {
                board[index][j] = num << 1;
                score += num << 1;
                num = 0;
                board[i][j] = 0;
                index--;
            }
            else {
                board[index][j] = num;
                num = board[i][j];
                board[i][j] = 0;
                index--;
            }
        }
        if (num != 0) board[index][j] = num;
    }
}
*/

/*
int moveDown(){
    int boardChanged = 0;
    for(int j = 0; j < 4; j++){
        int index = 3;
        int num = 0;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] != 0) {
                if (index != i) {
                    num = board[i][j];
                    board[i][j] = 0;
                    board[index][j] = num;
                    boardChanged = 1;
                }
                index--;
            }
        }
        index = 3;
        num = 0;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] == 0) continue;
            if (num == 0) {
                num = board[i][j];
                board[i][j] = 0;
                continue;
            }
            if (num == board[i][j]) {
                boardChanged = 1;
                board[index][j] = num << 1;
                score += num << 1;
                num = 0;
                board[i][j] = 0;
                index--;
            }
            else {
                board[index][j] = num;
                num = board[i][j];
                board[i][j] = 0;
                index--;
            }
        }
        if (num != 0) board[index][j] = num;
    }
    return boardChanged;
}
*/

int moveDown(){
    int boardChanged = 0;
    for(int j = 0; j < 4; j++){
        int num = 0;
        int indexFrom;
        int indexTo = 3;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] == 0) continue;
            if (num == 0) {
                num = board[i][j];
                board[i][j] = 0;
                indexFrom = i;
                continue;
            }
            if (num == board[i][j]) {
                boardChanged = 1;
                board[indexTo][j] = num << 1;
                score += num << 1;
                num = 0;
                board[i][j] = 0;
                indexTo--;
            }
            else {
                if (indexFrom != indexTo) boardChanged = 1;
                board[indexTo][j] = num;
                num = board[i][j];
                board[i][j] = 0;
                indexFrom = i;
                indexTo--;
            }
        }
        if (num != 0) {
            if (indexFrom != indexTo) boardChanged = 1;
            board[indexTo][j] = num;
        }
    }
    return boardChanged;
}

void moveRight(){}

/* IDEAS */
// best score or score board => file I/O