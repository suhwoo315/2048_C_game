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
int slideBlock();
int mergeBlock();

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

int moveDown(){
    int boardChanged = slideBlock();
    if (boardChanged == 0) boardChanged = mergeBlock();
    else mergeBlock();
    if (boardChanged == 0) boardChanged = slideBlock();
    else slideBlock();
    return boardChanged;
}

void moveRight(){}

int slideBlock(){
    int boardChanged = 0;
    for(int j = 0; j < 4; j++){
        int index = 3;
        for(int i = 3; i >= 0; i--){
            if (board[i][j] != 0) {
                if (index != i) {
                    board[index][j] = board[i][j];
                    board[i][j] = 0;
                    boardChanged = 1;
                }
                index--;
            }
        }
    }
    return boardChanged;
}

int mergeBlock(){
    int boardChanged = 0;
    for(int j = 0; j < 4; j++){
        for(int i = 3; i > 0; i--){
            if (board[i][j] == 0) continue;
            if (board[i][j] == board[i-1][j]) {
                score += board[i][j] << 1;
                board[i][j] = board[i][j] << 1;
                board[i-1][j] = 0;
                boardChanged = 1;
            }
        }
    }
    return boardChanged;
}

/* IDEAS */
// best score or score board => file I/O