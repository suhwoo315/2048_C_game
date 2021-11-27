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
void moveDown();
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
            moveDown();
            spawnBlock();
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

void moveDown(){
    int num1 = 0, num2 = 0;
    int index = 3;
    for(int j = 0; j < 4; j++){
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
        num1 = num2 = 0;
        for(int i = 0; i <= index; i++) board[i][j] = 0;
        index = 3;
    }
}

void moveRight(){}

/* IDEAS */
// best score or score board => file I/O
/*
    variable a, b to temporarily save number
    variable c for slot number

    if it is 1, don't do anything

    save first number to a, and second number to b
    if a = b,
        save bit-shifted value to slot c
        c++
        score += bit-shifted value
    if a != b,
        save a value to slot c
        c++
        move b to a
        save next number to b
*/