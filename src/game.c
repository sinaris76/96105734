#include "game.h"
#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include "physics.h"
#include <math.h>

void initiateGame(char* filename, Map* outMap, Game* outGame, Pacman* outPacman, Ghost* outGhosts) {
    filename="res\\map.txt";
    //it also can be reality.txt or simple.txt
    FILE *map;
    map=fopen(filename,"r");
    fscanf(map,"%d %d\n",&outMap->height,&outMap->width);
    int i,j;
    outGame->cheeses=0;
    outGame->cherries=0;
    outGame->pineapples=0;
    for (i=0 ; i<outMap->height ; i++)
    {
        for (j=0 ; j<outMap->width ; j++ ) {
            fscanf(map, "%c ", &outMap->cells[j][i]);

            if (outMap->cells[j][i]==CELL_CHEESE)
                outGame->cheeses++;
            else if (outMap->cells[j][i]==CELL_CHERRY)
                outGame->cherries++;
            else if (outMap->cells[j][i]==CELL_PINEAPPLE)
                outGame->pineapples++;
        }
        fscanf(map,"\n");
    }
    fscanf(map,"%d\n",&outGame->score);
    outGame->ghosts=4;
    fscanf(map,"%*s %d %d (%d,%d) (%lf,%lf)\n",&outPacman->dir,&outPacman->health,&outPacman->startY,&outPacman->startX,&outPacman->y,&outPacman->x);
    outPacman->speed=PACMAN_DEFAULT_SPEED;
    for (i=0 ; i<4 ; i++) {
        fscanf(map, "%*s");
        outGhosts[i].type = i;
        int temp;
        fscanf(map, "%d %d ", &outGhosts[i].dir,&temp);
        if (temp == 0) {
            outGhosts[i].blue=true;
            unsigned int temp2;
            fscanf(map, "%u (%d,%d) (%lf,%lf)",&temp2,&outGhosts[i].startY,&outGhosts[i].startX, &outGhosts[i].y, &outGhosts[i].x);
            outGhosts[i].blueCounterDown=temp2*CYCLES_PER_SEC;
        }
        else {
            outGhosts[i].blue=false;
            outGhosts[i].blueCounterDown=0;
            fscanf(map,"(%d,%d) (%lf,%lf)", &outGhosts[i].startY, &outGhosts[i].startX,& outGhosts[i].y,&outGhosts[i].x);
        }
        outGhosts[i].speed=GHOST_DEFAULT_SPEED;
    }
}

void checkEatables(Map* map, Game* outGame, Pacman* outPacman, Ghost* outGhosts) {
    int a,b;
    int temp=0;
    if (outPacman->dir==DIR_UP){
        if (outPacman->y-(int)(outPacman->y)<0.3){
            a = (int) outPacman->x;
            b = (int) outPacman->y;
            temp = 1;
        }
    }
    else if (outPacman->dir==DIR_DOWN){
        if ((int)outPacman->y-outPacman->y<0.3){
            a = (int) outPacman->x;
            b = (int) (outPacman->y)%map->height;
            temp = 1;
        }
    }
    else if(outPacman->dir==DIR_RIGHT){
        if ((int)outPacman->x-outPacman->x<0.3){
            a = (int) (outPacman->x)%map->width;
            b = (int) outPacman->y;
            temp = 1;
        }
    }
    else if (outPacman->dir==DIR_LEFT){
        if (outPacman->x-(int)outPacman->x<0.3) {
            a = (int) outPacman->x;
            b = (int) outPacman->y;
            temp = 1;
        }
    }
    if (temp==1) {
        switch (map->cells[a][b]) {
            case CELL_CHEESE:
                outGame->score += CHEESE_SCORE;
                map->cells[a][b] = CELL_EMPTY;
                outGame->cheeses--;
                break;
            case CELL_CHERRY:
                outGame->score += CHERRY_SCORE;
                map->cells[a][b] = CELL_EMPTY;
                outGame->cherries--;
                break;
            case CELL_PINEAPPLE:
                outGame->score += PINEAPPLE_SCORE;
                map->cells[a][b] = CELL_EMPTY;
                outGame->pineapples--;
                int i;
                for (i = 0; i < 4; i++) {
                    outGhosts[i].blue = 1;
                    outGhosts[i].blueCounterDown = BLUE_DURATION;
                }
                break;
        }
    }
}

void checkGhostCollision(Pacman* outPacman, Ghost* outGhost) {
    double distance_x,distance_y;
    distance_x = outPacman->x - outGhost->x ;
    distance_y = outPacman->y - outGhost->y ;
    double distance;
    distance=sqrt((pow(distance_x,2))+(pow(distance_y,2)));
    if (distance<0.6){
        if (outGhost->blue==1){
            outGhost->blue=0;
            outGhost->x = (double)outGhost->startX;
            outGhost->y = (double)outGhost->startY;
        }
        else{
            outPacman->health--;
            outPacman->x = (double)outPacman->startX;
            outPacman->y = (double)outPacman->startY;
        }
    }
}

bool isGameFinished(Game* game, Pacman* pacman) {
    if (game->cheeses==0 && game->pineapples==0)
        return 1;
    else if (pacman->health<=0)
        return 1;
    else
        return 0;
}

void checkGhostState(Ghost* ghost) {
    if (ghost->blue==1){
        if (ghost->blueCounterDown!=0)
            ghost->blueCounterDown--;
        else if (ghost->blueCounterDown==0)
            ghost->blue=0;
    }
}
