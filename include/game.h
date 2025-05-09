#ifndef GAME_H
#define GAME_H

#define WIDTH  45
#define HEIGHT 25

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int x, y;
}coordinates;

typedef struct {
    coordinates* body;
    int length;
    char direction;
}Snake;

extern Snake snake;
extern coordinates fruit;
extern uint16_t score;
extern bool running;
extern bool paused;

void initGame();
void printGame();
void updateGame();
void keyboardInput();
void run_game(const char*);

#endif
