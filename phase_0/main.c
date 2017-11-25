#include <stdio.h>
#include <stdlib.h>
//rows and columns
int n,m;

struct pacman{
	int direction;
	int heart;
	int first_x;
	int first_y;
	int current_x;
	int current_y;
};

struct ghost{
	int direction;
	int run_time;
	int situation;
	int first_x;
	int first_y;
	int current_x;
	int current_y;
};

void place(char map[n][m],struct pacman *temp_pac){
	//up
	if ((*temp_pac).direction==1){
		int temp;
		temp=((*temp_pac).current_x + n-1)%n;
		if (map[temp][(*temp_pac).current_y]!='#') 
			(*temp_pac).current_x=temp;		
	}
	//right
	else if ((*temp_pac).direction==2){
		int temp;
		temp=((*temp_pac).current_y+1)%m;
		if (map[(*temp_pac).current_x][temp]!='#')
			(*temp_pac).current_y=temp;
	}
	//down
	else if ((*temp_pac).direction==3){
		int temp;
		temp=((*temp_pac).current_x+1)%n;
		if (map[temp][(*temp_pac).current_y]!='#')
			(*temp_pac).current_x=temp;
	}
	//left
	else if ((*temp_pac).direction==4){
		int temp;
		temp=((*temp_pac).current_y+m-1)%m;
		if (map[(*temp_pac).current_x][temp]!='#')
			(*temp_pac).current_y=temp;
	}
}


int main(int argc, char *argv[]) {
	//rows and columns
	scanf("%d %d\n", &n , &m );
	//map
	char main_map[n][m];
	int i , j;
	for (i=0 ; i<n ; i++){
		for (j=0 ; j<m ; j++)
			scanf("%c",&main_map[i][j]);
		scanf("\n");}
	//time and score
	int t_min,t_sec;
	int main_score;
	scanf("%02d:%02d %d\n",&t_min,&t_sec,&main_score);
	//pacman
	struct pacman pac;
	scanf("pacman: %d %d (%d,%d) (%d,%d)\n", &pac.direction , &pac.heart , &pac.first_x , &pac.first_y , &pac.current_x , &pac.current_y);
	//blinky
	struct ghost blinky;
	scanf("blinky: %d %d ", &blinky.direction , &blinky.situation);
	if (blinky.situation==0)
		scanf("%d (%d,%d) (%d,%d)\n", &blinky.run_time , &blinky.first_x , &blinky.first_y , &blinky.current_x , &blinky.current_y );
	else
		scanf("(%d,%d) (%d,%d)\n", &blinky.first_x , &blinky.first_y , &blinky.current_x , &blinky.current_y );
	//pinky
	struct ghost pinky;
	scanf("pinky: %d %d ", &pinky.direction , &pinky.situation);
	if (pinky.situation==0)
		scanf("%d (%d,%d) (%d,%d)\n", &pinky.run_time , &pinky.first_x , &pinky.first_y , &pinky.current_x , &pinky.current_y );
	else
		scanf("(%d,%d) (%d,%d)\n", &pinky.first_x , &pinky.first_y , &pinky.current_x , &pinky.current_y );
	//clyde
	struct ghost clyde;
	scanf("clyde: %d %d ", &clyde.direction , &clyde.situation);
	if (clyde.situation==0)
		scanf("%d (%d,%d) (%d,%d)\n", &clyde.run_time , &clyde.first_x , &clyde.first_y , &clyde.current_x , &clyde.current_y );
	else
		scanf("(%d,%d) (%d,%d)\n", &clyde.first_x , &clyde.first_y , &clyde.current_x , &clyde.current_y );
	//inky
	struct ghost inky;
	scanf("inky: %d %d ", &inky.direction , &inky.situation);
	if (inky.situation==0)
		scanf("%d (%d,%d) (%d,%d)", &inky.run_time , &inky.first_x , &inky.first_y , &inky.current_x , &inky.current_y );
	else
		scanf("(%d,%d) (%d,%d)", &inky.first_x , &inky.first_y , &inky.current_x , &inky.current_y );
	
	if (pac.heart!=0)
		place(main_map,&pac);
	

	return 0;
}
