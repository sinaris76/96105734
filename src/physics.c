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

Direction BFS(const Map *map ,vertics *vertic ,int start,int end){
    int queue[map->width*map->height];
    int i=0;
    for (i=0 ; i<(map->width*map->height) ; i++)
        queue[i]=-1;
    i=1;
    queue[0]=start;
    int front=0;
    vertic[queue[front]].visit=true;
    vertic[queue[front]].level=0;
    while (queue[front]!=-1){
        int k;
        for (k=0 ; k<=3 ; k++){
            int tmp=vertic[queue[front]].adjanency_list[k];
            if (tmp!=-1 && vertic[tmp].visit==false){
                vertic[tmp].visit=true;
                vertic[tmp].level=vertic[queue[front]].level+1;
                //we store the first place we should go in order to be in this way to the end
                if (vertic[tmp].level==1)
                    vertic[tmp].start=tmp;
                else
                    vertic[tmp].start=vertic[queue[front]].start;
                if (tmp==end)
                    break;
                queue[i]=tmp;
                i++;
            }
        }
        front++;
    }
    int end_x,end_y,start_x,start_y;
    end_x=(vertic[end].start)%map->width;
    end_y=((vertic[end].start)-end_x)/map->width;
    start_x=start%map->width;
    start_y=(start-start_x)/map->width;
    if (start_x==end_x){   //up & down
        if(end_y==(start_y+1)%map->height)
            return DIR_DOWN;
        else if(end_y==(start_y-1+map->height)%map->height)
            return DIR_UP;
    }
    else if (start_y==end_y) {    //right & left
        if (end_x == (start_x + 1) % map->width)
            return DIR_RIGHT;
        else if (end_x == (start_x - 1 + map->width) % map->width)
            return DIR_LEFT;
    }

}

int pinky_end(const Map* map,Ghost *ghost ,Pacman *pacman){
    int end;
    if(pacman->dir==DIR_RIGHT){
        int k;
        for (k=1 ; k<=4 ; k++)
            if (map->cells[((int)pacman->x+k+map->width)%map->width][(int)pacman->y]==CELL_BLOCK)
                break;
        end=(int)pacman->y*map->width+((int)pacman->x+k-1+map->width)%map->width;
    }
    else if (pacman->dir==DIR_LEFT){
        int k;
        for (k=1 ; k<=4 ; k++)
            if (map->cells[((int)pacman->x-k+map->width)%map->width][(int)pacman->y]==CELL_BLOCK)
                break;
        end=(int)pacman->y*map->width+((int)pacman->x-k+1+map->width)%map->width;
    }
    else if (pacman->dir==DIR_UP){
        int k;
        for (k=1 ; k<=4 ; k++)
            if (map->cells[(int)pacman->x][((int)pacman->y-k+map->height)%map->height]==CELL_BLOCK)
                break;
        end=(((int)pacman->y-k+1+map->height)%map->height)*map->width+(int)pacman->x;
    }
    else if (pacman->dir==DIR_DOWN){
        int k;
        for (k=1 ; k<=4 ; k++)
            if (map->cells[(int)pacman->x][((int)pacman->y+k+map->height)%map->height]==CELL_BLOCK)
                break;
        end=(((int)pacman->y+k-1+map->height)%map->height)*map->width+(int)pacman->x;
    }
    return end;
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
