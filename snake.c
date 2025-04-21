#include "snake.h"

int map_width = INITIAL_MAP_WIDTH;
int map_height = INITIAL_MAP_HEIGHT;
int obstacles_x[MAX_OBSTACLES];
int obstacles_y[MAX_OBSTACLES];
int num_obstacles = 0;
char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH + 1];
char key;
char last_key = 'd';
int stars_collected = 0;
int speed;
int x = 5, y = 11;
int ox, oy;
int star_x = 8, star_y = 3;
struct snakestail snakestail_old[MAP_SNAKE_LENGTH];
