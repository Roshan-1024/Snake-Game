#include "../include/game.h"
#include "../include/scores.h"
#include "../include/common.h"

#include<time.h>

#if defined(__linux__) || defined(__APPLE__)
#include <ncurses.h>
#include <unistd.h>

#elif defined(_WIN32)
#include<windows.h>
#include <conio.h>

#endif

#define WIDTH  45
#define HEIGHT 25
#define MAX_LENGTH 1000 // max length of snake body

Snake snake;
coordinates fruit;
uint16_t score; // Range: 0 to 65535
bool running;
bool paused;

void initGame(){
    #if defined(__linux__) || defined(__APPLE__)
    // ncurses FUNCTIONS (for Linux or macOS)
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    #endif

    /////////////////////////////////////////////////////////////////
    //                      GAME SETTINGS
    /////////////////////////////////////////////////////////////////

    // SNAKE DEFAULT PROPERTIES
    snake.length = 1;
    snake.direction = 'l'; // Right
    snake.body[0].x = WIDTH/2; // Initial x coordinate of snake at center
    snake.body[0].y = HEIGHT/2; // Initial y coordinate of snake at center

    // FRUIT PROPERTIES
    srand(time(NULL));
    do
    {
        fruit.x = rand()%(WIDTH-2) + 1; // Random fruit spawn x-coordinate
        fruit.y = rand()%(HEIGHT-2) + 1; // Random fruit spawn y-coordinate
    } while (snake.body[0].x == fruit.x && snake.body[0].y == fruit.y);
    
    // MISC.
    running = true;
    paused = false;
    score = 0;   
}

void printGame(){
    #if defined(__linux__) || defined(__APPLE__)
   
    clear();
    
    // DISPLAY THE SCORE
    mvprintw(0,(WIDTH/2),"Score: %hu",score);

    // PRINTING THE BOARD
    for(int y = 0; y <= HEIGHT+2; y++){
        for(int x = 0; x < WIDTH; x++){
            if(y == 1)  mvprintw(y, x, "_");
            if(x == 0 && y >= 2 && y < HEIGHT+2)  mvprintw(y, x, "|");
            if(x == WIDTH-1 && y >= 2 && y < HEIGHT+2)    mvprintw(y, x, "|");
            if(y == HEIGHT+2)   mvprintw(y, x, "-");
        }
    }

    // PRINTING THE SNAKE
    for (int i=0;i<snake.length;i++){
        if(i==0) mvprintw(snake.body[i].y+2,snake.body[i].x,"@");
        else mvprintw(snake.body[i].y+2,snake.body[i].x,"o");
    }

    // PRINTING THE FRUIT
    mvprintw(fruit.y+2,fruit.x,"+");

    refresh();

    #elif defined(_WIN32)
    // Alternative for Windows
    system("cls");
    
    // PRINTING THE BOARD
    for(int i = 0; i <= HEIGHT+2; i++){
        for(int j = 0; j < WIDTH; j++){
            bool printed = false;
            if(i == 0 && j == (WIDTH/2) - 4) { printf("Score: %hu",score); printed = true; }    // DISPLAY THE SCORE
            if(i == 1) { printf("_"); printed = true; } 
            if(j == 0 && i >= 2 && i < HEIGHT+2) { printf("|"); printed = true; }
            if(j == WIDTH-1 && i >= 2 && i < HEIGHT+2) { printf("|"); printed = true; }
            if(i == HEIGHT+2) { printf("-"); printed = true; }
            
            // PRINTING THE SNAKE
            for (int k=0;k<snake.length;k++){
                if(i == snake.body[k].y+2 && j == snake.body[k].x){
                    (k==0) ?  printf("@") : printf("o") ;
                    printed = true;
                }
            }
            // PRINTING THE FRUIT
            if(i == fruit.y+2 && j == fruit.x) {
                printf("+");
                printed = true;
            }
            if(printed == false)  printf(" ");
        }
        printf("\n");
    }    
    #endif
}

void updateGame(){
    if(paused) return;

    /////////////////////////////////////////////////////////////
    //                      UPDATE SNAKE
    /////////////////////////////////////////////////////////////

    // MAKE THE BODY ELEMENT FOLLOW ITS LEADING ELEMENT
    for(int i = snake.length-1; i > 0; i--)
        snake.body[i] = snake.body[i-1];

    // SET THE NEW SNAKE HEAD DIRECTION
    switch(snake.direction){
        case 'k': snake.body[0].y--; break;
        case 'j': snake.body[0].y++; break;
        case 'h': snake.body[0].x--; break;
        case 'l': snake.body[0].x++; break;
    }

    /////////////////////////////////////////////////////////////
    //                      CHECK COLLISIONS
    /////////////////////////////////////////////////////////////

    // COLLISION WITH WALLS CHECK
    if(snake.body[0].x == 0 || snake.body[0].x == WIDTH - 1 ||
        snake.body[0].y == -1 || snake.body[0].y == HEIGHT)
        running = false;
    
    // COLLISION WITH ITSELF CHECK
    for(int i = 1; i < snake.length; i++){
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y){
            running = false;
        }
    }

    /////////////////////////////////////////////////////////////
    //                      UPDATE SCORE
    /////////////////////////////////////////////////////////////
    if(snake.body[0].x == fruit.x && snake.body[0].y == fruit.y){
        score += 10;
        if(snake.length<MAX_LENGTH) snake.length++; // to prevent overflow   
        fruit.x = rand()%(WIDTH-2) + 1;
        fruit.y = rand()%(HEIGHT-2) + 1;
        for (int i = 0; i < snake.length; i++) {  //if food is spawned on snake's body food will be respawned till when it is not on snake's body
            if (snake.body[i].x == fruit.x && snake.body[i].y == fruit.y) {   
                fruit.x = rand()%(WIDTH-2) + 1;
                fruit.y = rand()%(HEIGHT-2) + 1;
                i=0;
            }
        }
    } 
}

void keyboardInput(){
    char dir;

    #if defined(__linux__) || defined(__APPLE__)
    dir = getch();
    
    #elif defined(_WIN32)
    int wait_time = 100; // 100 milliseconds

    for(int i = 0; i < wait_time; i+=10){
        if (_kbhit()) { // Check if a key is pressed
            dir = _getch(); // Read the key
            break;; // Exit after key press
        }
        Sleep(10); // Wait 10ms and then again checks if a key is pressed
    }

    #endif
    
    switch(dir){
        case 'k': // up
            if (snake.direction != 'k' && snake.direction != 'j') snake.direction = 'k'; 
            break;
        case 'j': // down
            if (snake.direction != 'j' && snake.direction != 'k') snake.direction = 'j';
            break;
        case 'h': // left
            if (snake.direction != 'h' && snake.direction != 'l') snake.direction = 'h';
            break;
        case 'l': // right
            if (snake.direction != 'l' && snake.direction != 'h') snake.direction = 'l';
            break;
        case 'p': // Toggle Pause and Play
            paused = !paused;
            break;
        case 'q': // quit game
            running = false;
            break;
    }
}

void run_game(const char* user){
    char choice;
    do{
        initGame(); // Applies default settings for the game
        while(running){
            printGame(); // Prints Board, Snake and Fruit
            keyboardInput(); // Takes the user input
            updateGame();
            #if defined(__linux__) || defined(__APPLE__)
            usleep(100000);  //pause execution 100 millisecond  (ncurses function)
            #elif defined(_WIN32)
            Sleep(100);  //pause execution 100 millisecond (windows.h function)
            #endif
        }
        #if defined(__linux__) || defined(__APPLE__)
        endwin(); // to return to console (ncurses function)
        #endif

        printf("\nGame Over!\n%s Your Score: %hu\n",user,score);
        updatehighscore(user, score); // Also, print if new highscore

        printf("Do you want to play again? (Y/N): ");
        scanf(" %c", &choice);
    }while(choice == 'y' || choice == 'Y');

    printf("Exiting the game...\n");
}