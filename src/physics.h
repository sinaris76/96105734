#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "game.h"
#include "input.h"

#define PACMAN_DEFAULT_SPEED 2.
#define GHOST_DEFAULT_SPEED 2.

// TO WRITE
Direction decidePacman(const Map* map, Pacman* pacman, Action action);

// TO WRITE
Direction decideGhost(const Map* map, Ghost* ghost,Pacman* pacman,Ghost* Blinky);
#endif
//me writing
typedef struct {
    int adjanency_list[4];
    int level;
    bool visit;
    int start;

}vertics;

void edge(const Map* map ,vertics *vertic);

Direction BFS(const Map *map ,vertics *vertic ,int start,int end);

int pinky_end(const Map* map,Ghost *ghost ,Pacman *pacman);


