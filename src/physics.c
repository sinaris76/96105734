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

Direction decideGhost(const Map* map, Ghost* ghost, Pacman* pacman,Ghost* Blinky){
    vertics vertic[map->width*map->height];
    edge(map,vertic);
    int start, end;
    int check=0;
    start=(int)ghost->y*map->width +(int) ghost->x;
    if (ghost->blue==true)
        end=(ghost->startY)*(map->width)+ghost->startX;
    else{
        if (ghost->type==BLINKY)
            end=(int)pacman->y*map->width+ (int)pacman->x;
        else if (ghost->type==PINKY){
            end=pinky_end(map,ghost,pacman);
        }
        else if (ghost->type==CLYDE){
            double distance_x,distance_y,distance;
            distance_x=pacman->x-ghost->x;
            distance_y=pacman->y-ghost->y;
            distance=abs((int)distance_x)+abs((int)distance_y);
            if (distance>8)
                end=(int)pacman->y*map->width+ (int)pacman->x;
            else{
                if (map->cells[0][map->height-1]!=CELL_BLOCK)
                    end=(map->height-1)*map->width;
                else
                    check=1;
            }
        }
        else if (ghost->type==INKY){
            int center_x,center_y,end_x,end_y;
            if (pacman->dir==DIR_RIGHT){
                center_y=(int)pacman->y;
                center_x=((int)pacman->x+2+map->width)%map->width;
            }
            else if (pacman->dir==DIR_LEFT){
                center_y=(int)pacman->y;
                center_x=((int)pacman->x-2+map->width)%map->width;
            }
            else if (pacman->dir==DIR_UP){
                center_x=(int)pacman->x;
                center_y=((int)pacman->y-2+map->height)%map->height;
            }
            else {
                center_x=(int)pacman->x;
                center_y=((int)pacman->y+2+map->height)%map->height;
            }
            end_x=(2*center_x-(int)Blinky->x+map->width)%map->width;
            end_y=(2*center_y-(int)Blinky->y+map->height)%map->height;
            if (map->cells[end_x][end_y]!=CELL_BLOCK)
                end=end_y*map->width+end_x;
            else{
                check=1;}
        }
    }

    if (check==1 || end==start){
        static int x[4]={1,1,1,1};
        while(1){
        x[ghost->type]=x[ghost->type]%4+1;
        if (x[ghost->type]-1==DIR_DOWN && map->cells[(int)ghost->x][(int)(ghost->y+1)%map->height]!=CELL_BLOCK)
            return DIR_DOWN;
        else if (x[ghost->type]-1==DIR_UP && map->cells[(int)ghost->x][ghost->y>0?(int)(ghost->y-1+map->height)%map->height:map->height-1]!=CELL_BLOCK)
            return DIR_UP;
        else if (x[ghost->type]-1==DIR_RIGHT && map->cells[(int)(ghost->x+1)%map->width][(int)ghost->y]!=CELL_BLOCK)
            return DIR_RIGHT;
        else if (x[ghost->type]-1==DIR_LEFT && map->cells[ghost->x>0?(int)(ghost->x-1+map->width)%map->width:map->width-1][(int)ghost->y]!=CELL_BLOCK)
            return DIR_LEFT;
        else
            continue;}
    }
    return   BFS(map,vertic,start,end);
}

Direction decidePacman(const Map* map, Pacman* pacman, Action action) {
    if (action==ACTION_UP ) {
         if ( map->cells[(int) pacman->x][pacman->y>0?(int)(pacman->y-1) + map->height % map->height:map->height-1] != CELL_BLOCK)
             return DIR_UP;
         else{
             if (pacman->dir==DIR_DOWN && map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
                 return DIR_DOWN;
             else if (pacman->dir==DIR_LEFT && map->cells[pacman->x>0?(int)(pacman->x-1+map->width)%map->width:map->width-1][(int)pacman->y]!=CELL_BLOCK)
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
             if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][pacman->y>0?(int)(pacman->y-1+map->height)%map->height:map->height-1]!=CELL_BLOCK)
                 return DIR_UP;
             else if (pacman->dir==DIR_LEFT && map->cells[pacman->x>0?(int)(pacman->x-1+map->width)%map->width:map->width-1][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_LEFT;
             else if (pacman->dir==DIR_RIGHT && map->cells[(int)(pacman->x+1)%map->width][(int)pacman->y]!=CELL_BLOCK)
                 return DIR_RIGHT;
             else
                 return DIR_NONE;
             }
    }
    else if (action==ACTION_LEFT ){
        if (map->cells[pacman->x>0?(int)(pacman->x-1)+map->width%map->width:map->width-1][(int)pacman->y]!=CELL_BLOCK)
            return DIR_LEFT;
        else{
            if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][pacman->y>0?(int)(pacman->y-1) + map->height % map->height:map->height-1]!=CELL_BLOCK)
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
            if (pacman->dir==DIR_UP && map->cells[(int)pacman->x][pacman->y>0?(int)(pacman->y-1) + map->height % map->height:map->height-1]!=CELL_BLOCK)
                return DIR_UP;
            else if (pacman->dir==DIR_DOWN && map->cells[(int)pacman->x][(int)(pacman->y+1)%map->height]!=CELL_BLOCK)
                return DIR_DOWN;
            else if (pacman->dir==DIR_LEFT && map->cells[pacman->x>0?(int)(pacman->x-1+map->width)%map->width:map->width-1][(int)pacman->y]!=CELL_BLOCK)
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
