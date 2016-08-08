#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 10
#define NUM_OBSTACLES 2
#define LENGTH 50

int sleep_time = 200000;//in milliseconds

char grid[LENGTH][LENGTH];

void create_grid();
void print_grid();
void *move_drone(void *threadarg);
void set_obstacles();
void go_around(int*, int*, char, int);

struct thread_data{
  int thread_id;
  int sx,sy;
  int dx,dy;
};
struct thread_data thread_data_array[NUM_THREADS];

struct obstacles{
    int x;
    int y;
};
struct obstacles obstacle_array[NUM_OBSTACLES];

int main (int argc, char *argv[])
{
  create_grid();
  set_obstacles();
  
  pthread_t threads[NUM_THREADS]; //Thread Address
  //int *taskids[NUM_THREADS];
  int rc; //Holds thread return code
  long t; //Loop Counter
  for(t=0;t<NUM_THREADS;t++) {

    thread_data_array[t].thread_id = t;
    thread_data_array[t].sx = t;
    thread_data_array[t].sy = t;
    thread_data_array[t].dx = t+10;
    thread_data_array[t].dy = t+10;

    rc = pthread_create(&threads[t], NULL, move_drone, (void *) 
			&thread_data_array[t]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  /* Last thing that main() should do */
  pthread_exit(NULL);
}

void go_around(int* cx, int* cy, char direction, int taskid){
    
    if(direction == '>'){
            grid[(*cx)++][(*cy)] = taskid;//down
            grid[(*cx)][(*cy)++] = taskid;//right
            grid[(*cx)][(*cy)++] = taskid;//right
            grid[(*cx)--][(*cy)] = taskid;//up
             
    }
    if(direction == 'V'){
            grid[(*cx)][(*cy)--] = taskid;//left
            grid[(*cx)++][(*cy)] = taskid;//down
            grid[(*cx)++][(*cy)] = taskid;//down
            grid[(*cx)][(*cy)++] = taskid;//right
        
    }
    if(direction == '^'){
            grid[(*cx)][(*cy)--] = taskid;//left
            grid[(*cx)--][(*cy)] = taskid;//up
            grid[(*cx)--][(*cy)] = taskid;//up
            grid[(*cx)][(*cy)++] = taskid;//right
        
    }
    if(direction == '<'){
            grid[(*cx)++][(*cy)] = taskid;//down
            grid[(*cx)][(*cy)--] = taskid;//left
            grid[(*cx)][(*cy)--] = taskid;//left
            grid[(*cx)--][(*cy)] = taskid;//up
        
    }
}

void set_obstacles(){
    obstacle_array[0].x = 0;
    obstacle_array[0].y = 5;
    obstacle_array[1].x = 10;
    obstacle_array[2].x = 20;
    
    for(int i = 0; i < NUM_OBSTACLES; i++){
        int x = obstacle_array[i].x;
        int y = obstacle_array[i].y;
        
        grid[x][y]= 'X';
    }
}

void *move_drone(void *threadarg){
  int taskid;
  int start_x, start_y;
  int dest_x, dest_y;
  struct thread_data *my_data;

  my_data = (struct thread_data *) threadarg;
  taskid = my_data->thread_id;
  taskid += 48 ;
  start_x = my_data->sx;
  start_y = my_data->sy;
  dest_x = my_data->dx;
  dest_y = my_data->dy;
  
  grid[start_x][start_y] = taskid;
  print_grid();
  
  int curr_x = start_x;
  int curr_y = start_y;
  
  char direction = '>';
  for(;curr_y < dest_y; curr_y++ ){
      if(grid[curr_x][curr_y+1] == 'X'){
        go_around(&curr_x,&curr_y, direction, taskid);
      }
      grid[curr_x][curr_y+1] = taskid;
      grid[curr_x][curr_y] = direction;
      print_grid();
      
      usleep(sleep_time);
      
  }
  
  direction = 'V';
  for(;curr_x < dest_x; curr_x++ ){
      if(grid[curr_x+1][curr_y] == 'X'){
        go_around(&curr_x,&curr_y, direction, taskid);
      }
      grid[curr_x+1][curr_y] = taskid;
      grid[curr_x][curr_y] = direction;
      print_grid();
      usleep(sleep_time);
      
  }
  
  direction = '^';
  for(;curr_x > start_x; curr_x-- ){ 
      if(grid[curr_x-1][curr_y] =='X'){
        go_around(&curr_x,&curr_y, direction, taskid);
      }
      grid[curr_x-1][curr_y] = taskid;
      grid[curr_x][curr_y] = direction;
      print_grid();
      usleep(sleep_time);
      
  }
  
  direction = '<';
  for(;curr_y > start_y; curr_y-- ){
      if(grid[curr_x][curr_y-1] == 'X'){
        go_around(&curr_x,&curr_y, direction, taskid);
      }
      grid[curr_x][curr_y-1] = taskid;
      grid[curr_x][curr_y] = direction;
      print_grid();
      usleep(sleep_time);
      
  }
  pthread_exit(NULL);
}

void print_grid(){
 
	printf("\e[1;1H\e[2J");
	for(int i = 0; i < LENGTH; i++){
		for (int j = 0; j < LENGTH; j++){
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
	return;
}

void create_grid(){
    for (int i = 0; i < LENGTH; i++){
		for (int j = 0; j < LENGTH; j++){
			grid[i][j] = 178;
		}
	}

	return;
}
