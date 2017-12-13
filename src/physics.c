#include "physics.h"
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "game.h"

Direction decideGhost(const Map* map, Ghost* ghost) {
    int x;
    x=(rand()%(5));
    if (x==0)
        return DIR_NONE;
    else if (x==DIR_DOWN && map->cells[(int)ghost->x][(int)(ghost->y+1)%map->height]!=CELL_BLOCK)
        return DIR_DOWN;
    else if (x==DIR_UP && map->cells[(int)ghost->x][(int)(ghost->y-1+map->height)%map->height]!=CELL_BLOCK)
        return DIR_UP;
    else if (x==DIR_RIGHT && map->cells[(int)(ghost->x+1)%map->width][(int)ghost->y]!=CELL_BLOCK)
        return DIR_RIGHT;
    else if (x==DIR_LEFT && map->cells[(int)(ghost->x-1+map->width)%map->width][(int)ghost->y]!=CELL_BLOCK)
        return DIR_LEFT;
    else
        decideGhost(map,ghost);


}

Direction decidePacman(const Map* map, Pacman* pacman, Action action) {
    if (action==ACTION_UP ) {
         if ( map->cells[(int) pacman->x][(int) (pacman->y - 1 + map->height) % map->height] != CELL_BLOCK)
             return DIR_UP;
         else{
             if (pacman->dir==DIR_DOWN && map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
                 return DIR_DOWN;
             else if (pacman->dir==DIR_LEFT && map->cells[(int)(pacman->x-1+map->width)%map->width][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_LEFT;
             else if (pacman->dir==DIR_RIGHT && map->cells[(int)(pacman->x+1)%map->width][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_RIGHT;
             else
                 return DIR_NONE;
         }
    }
    else if (action==ACTION_DOWN ){
         if ( map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
             return DIR_DOWN;
         else{
             if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][(int)(pacman->y-1+map->height)%map->height]!=CELL_BLOCK)
                 return DIR_UP;
             else if (pacman->dir==DIR_LEFT && map->cells[(int)(pacman->x-1+map->width)%map->width][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_LEFT;
             else if (pacman->dir==DIR_RIGHT && map->cells[(int)(pacman->x+1)%map->width][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_RIGHT;
             else
                 return DIR_NONE;
             }
    }
    else if (action==ACTION_LEFT ){
        if (map->cells[(int)(pacman->x-1+map->width)%map->width][(int)pacman->y]!=CELL_BLOCK)
            return DIR_LEFT;
        else{
            if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][(int)(pacman->y-1+map->height)%map->height]!=CELL_BLOCK)
                return DIR_UP;
            else if (pacman->dir==DIR_DOWN && map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
                return DIR_DOWN;
            else if (pacman->dir==DIR_RIGHT && map->cells[(int)(pacman->x+1)%map->width][(int)pacman->y]!=CELL_BLOCK)
                return DIR_RIGHT;
            else
                return DIR_NONE;
              }
    }
    else if (action==ACTION_RIGHT){
        if (map->cells[(int)(pacman->x+1)%map->width][(int)pacman->y]!=CELL_BLOCK)
            return DIR_RIGHT;
        else{
            if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][(int)(pacman->y-1+map->height)%map->height]!=CELL_BLOCK)
                return DIR_UP;
            else if (pacman->dir==DIR_DOWN && map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
                return DIR_DOWN;
            else if (pacman->dir==DIR_LEFT && map->cells[(int)(pacman->x-1+map->width)%map->width][(int)pacman->y]!=CELL_BLOCK)
                return DIR_LEFT;
            else
                return DIR_NONE;
             }
    }
    else if (action & MOVE_ACTION_MASK==0)
        return pacman->dir;
    else
        return DIR_NONE;
}
