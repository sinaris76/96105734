#include <stdio.h>
#include <game.h>
#include <map.h>
#include <physics.h>
#include <input.h>
#include <time.h>
#include ".\windows\sdl2-include\SDL.h"
#include ".\windows\sdl2-gfx-include\SDL2_gfxPrimitives.h"
#ifdef main
# undef main
#endif /* main */
int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO))
    {
              printf ("SDL_Init Error: %s", SDL_GetError());
              return 1;
           }
    Map* map=malloc(sizeof(Map));
    Game* game=malloc(sizeof(game));
    Pacman* pacman=malloc(sizeof(pacman));
    Ghost ghost[4];
    char file[50];
    initiateGame(file ,map , game , pacman , ghost);
    SDL_Window *window=SDL_CreateWindow("pacman",0,0,map->width*100,map->height*100+100,SDL_WINDOW_OPENGL);
    if (window==NULL)
        return 2;
    SDL_Renderer *renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer==NULL){
        return  3;
    }
    int i,j;
    int k=1;
    int dx[4]={0,1,0,-1};
    int dy[4]={-1,0,1,0};
    int quit=0;
    int move=0;
    long ghost_colors[4]={0xFF3C14DC,0xFFB469FF,0xFF7AA0FF,0xFFFACE87};
    Direction g_next[4];
    int g_move[4]={1,1,1,1};
    int g[4]={1,1,1,1};
    SDL_Event e;
    Direction p_next=DIR_NONE;
    Action next=MOVE_ACTION_MASK;
    time_t  start,then,dur;
    start=time(0);
    while(!quit){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        int a;
        a=isGameFinished(game,pacman);
        if (a==1){
            if (pacman->health<=0)
                stringRGBA(renderer,(map->width*100)/2-65,(map->height*100)/2+50,"You lost!",255,0,0,255);
            else
                stringRGBA(renderer,(map->width*100)/2-65,(map->height*100)/2+50,"You won!",255,0,0,255);
            quit=1;
        }
        else{
            thickLineRGBA(renderer,0,50,map->width*100,50,1,250,50,0,255);
            thickLineRGBA(renderer,0,map->height*100+50,map->width*100,map->height*100+50,1,250,50,0,255);
            stringRGBA(renderer,map->width*100-150,25,"score : ",250,50,0,255);
            char s_score[50];
            int b=game->score;
            sprintf(s_score,"%d",b);
            stringRGBA(renderer,map->width*100-100,25,s_score,250,50,0,255);
            then=time(0);
            dur=then-start;
            int min=dur/60;
            int sec=dur%60;
            char s_min[10];
            char s_sec[2];
            sprintf(s_min,"%d",min);
            sprintf(s_sec,"%.2d",sec);
            stringRGBA(renderer,(map->width*100)/2-100,25,"time : ",250,50,0,255);
            stringRGBA(renderer,(map->width*100)/2-50,25,s_min,250,50,0,255);
            stringRGBA(renderer,(map->width*100)/2-48,25,":",250,50,0,255);
            stringRGBA(renderer,(map->width*100)/2-46,25,s_sec,250,50,0,255);
            stringRGBA(renderer,0,75+map->height*100,"press q to quit",250,50,0,255);

            for (i=0 ; i<pacman->health ; i++)
                filledPieColor(renderer,i*50+50,35,20,240,300,0xFF8515C7);
            for (i=0 ; i <map->width ; i++)
                for (j=0 ; j<map->height ; j++){
                    if (map->cells[i][j]==CELL_BLOCK)
                        boxRGBA(renderer,i*100,j*100+50,(i+1)*100,(j+1)*100+50,250,50,0,255);
                    else if (map->cells[i][j]==CELL_CHEESE)
                        filledPieColor(renderer,i*100+30,j*100+100,40,-30,30,0xFFADDEFF);
                    else if (map->cells[i][j]==CELL_PINEAPPLE)
                        filledCircleColor(renderer,i*100+50,j*100+100,25,0xFF00A5FF);
                    else if (map->cells[i][j]==CELL_CHERRY)
                        filledCircleColor(renderer,i*100+50,j*100+100,15,0xFF8515C7);
                }
            filledPieColor(renderer,pacman->x*100+50,pacman->y*100+100,30,-45+(pacman->dir-1)*90,225+(pacman->dir-1)*90,0xFF7280FA);
            //ghosts
            for (i=0 ; i<4 ; i++){
                if (ghost[i].blue==1)
                    filledEllipseColor(renderer,ghost[i].x*100+50,ghost[i].y*100+100,25,40,0xFFCD0000);
                else
                    filledEllipseColor(renderer,ghost[i].x*100+50,ghost[i].y*100+100,25,40,ghost_colors[i]);
                filledEllipseColor(renderer,ghost[i].x*100+40,ghost[i].y*100+75,5,7,0xFFFFFFFF);
                filledEllipseColor(renderer,ghost[i].x*100+60,ghost[i].y*100+75,5,7,0xFFFFFFFF);
                filledCircleColor(renderer,ghost[i].x*100+60+dx[ghost[i].dir-1]*3,ghost[i].y*100+75+dy[ghost[i].dir-1]*3,4,0xFF000000);
                filledCircleColor(renderer,ghost[i].x*100+40+dx[ghost[i].dir-1]*3,ghost[i].y*100+75+dy[ghost[i].dir-1]*3,4,0xFF000000);
            }
            if (SDL_PollEvent(&e)){
                 if(e.type==SDL_KEYDOWN){
                     move=1;
                    switch (e.key.keysym.sym){
                        case SDLK_UP : next=ACTION_UP;break;
                        case SDLK_DOWN : next=ACTION_DOWN;break;
                        case SDLK_RIGHT : next=ACTION_RIGHT;break;
                        case SDLK_LEFT : next=ACTION_LEFT;break;
                        case SDLK_q:SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            SDL_Quit();
                            return 0;
                    }}
            }
            //pacman_move
            if (pacman->x-(int)pacman->x<0.01  && pacman->y-(int)pacman->y<0.01 && move==1 && pacman->x>=-0.01 && pacman->y>=-0.01){
                k=1;
                p_next=decidePacman(map,pacman,next);
                if (p_next==DIR_NONE)
                    move=0;
                else
                    pacman->dir=p_next;
            }
            if (move){
                pacman->x=pacman->x+dx[pacman->dir-1]*0.01;
                pacman->y=pacman->y+dy[pacman->dir-1]*0.01;
                if (pacman->x<=-0.5 && pacman->dir==DIR_LEFT)
                    pacman->x+=map->width;
                else if (pacman->x>=map->width-0.5 && pacman->dir==DIR_RIGHT)
                    pacman->x=0;
                if (pacman->y<=-0.25 && pacman->dir==DIR_UP)
                    pacman->y+=map->height-0.5;
                else if (pacman->y>=map->height-0.75 && pacman->dir==DIR_DOWN)
                    pacman->y=-0.25;
                k++;
                if (k==101)
                    k=1;
                }
            checkEatables(map,game,pacman,ghost);
            //ghost_moves
            for (i=0 ; i<4; i++){
                checkGhostState(&ghost[i]);
                if ((ghost[i].x-(int)ghost[i].x<=0.01) && (ghost[i].y-(int)ghost[i].y<=0.01) && ghost[i].x>=-0.01 && ghost[i].y>=-0.01){
                  //  printf("           %d\n",ghost[i].type);
                    g[i]=1;
                    g_next[i]=decideGhost(map,&ghost[i],pacman,&ghost[0]);
                    ghost[i].dir=g_next[i];
                }
                    ghost[i].x=ghost[i].x+dx[ghost[i].dir-1]*0.01;
                    ghost[i].y=ghost[i].y+dy[ghost[i].dir-1]*0.01;
                    if (ghost[i].x<=-0.5 && ghost[i].dir==DIR_LEFT)
                        ghost[i].x+=map->width;
                    else if (ghost[i].x>=map->width-0.5 && ghost[i].dir==DIR_RIGHT)
                        ghost[i].x=0;
                    if (ghost[i].y<=-0.25 && ghost[i].dir==DIR_UP)
                        ghost[i].y+=map->height-0.5;
                    else if (ghost[i].y>=map->height-0.75 && ghost[i].dir==DIR_DOWN)
                        ghost[i].y=-0.25;
                    g[i]++;
                    if (g[i]==101)
                        g[i]=1;
            }
            int check[4]={0,0,0,0};
            int l=0;
            while (l<4){
                if(ghost[l].blue==1){
                    check[l]=1;
                    checkGhostCollision(pacman,&ghost[l]);}
                l++;
            }
            for (l=0 ; l<4 ; l++){
                if (check[l]==0)
                    checkGhostCollision(pacman,&ghost[l]);
            }
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(7);
    }
    SDL_Delay(2000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}