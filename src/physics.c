#include "physics.h"
#include <time.h>
#include <stdlib.h>
#include "map.h"
#include "game.h"
#include <math.h>
#include <stdio.h>

void edge(const Map* map ,vertics *vertic){
    int i=0,j=0,k=0;
    for (i=0 ; i<map->width ; i++)
        for (j=0 ; j<map->height ; j++){
            vertic[j*map->width+i].visit=false;
            vertic[j*map->width+i].level=-1;
            if (map->cells[(i+1)%map->width][j]!= CELL_BLOCK){     //right
                vertic[j*map->width + i].adjanency_list[k]=j*map->width + ((i+1)%map->width);
                k++;
            }
            if (map->cells[(i-1+map->width)%map->width][j]!= CELL_BLOCK){    //left
                vertic[j*map->width + i].adjanency_list[k]=j*map->width + ((i-1+map->width)%map->width);
                k++;
            }
            if (map->cells[i][(j-1+map->height)%map->height]!= CELL_BLOCK){    //up
                vertic[j*map->width + i].adjanency_list[k]=((j-1+map->height)%map->height)*map->width + i;
                k++;
            }
            if (map->cells[i][(j+1)%map->height]!= CELL_BLOCK){    //down
                vertic[j*map->width + i].adjanency_list[k]=((j+1)%map->height)*map->width + i;
                k++;
            }
            for ( ; k<=3 ; k++)
                vertic[j*map->width + i].adjanency_list[k]=-1;
            k=0;
        }
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
