#ifndef SNAKE_H
#define SNAKE_H

#define MAX_OBSTACLES 100

#define MAP_SNAKE_LENGTH 216
#define INITIAL_MAP_WIDTH 20
#define INITIAL_MAP_HEIGHT 14
#define MAX_MAP_WIDTH 40
#define MAX_MAP_HEIGHT 28

#define WALL_COLOR 1
#define SNAKE_COLOR 2
#define STAR_COLOR 3
#define TEXT_COLOR 4

extern int map_width;
extern int map_height;

extern int obstacles_x[MAX_OBSTACLES];
extern int obstacles_y[MAX_OBSTACLES];
extern int num_obstacles;

extern char map[MAX_MAP_HEIGHT][MAX_MAP_WIDTH + 1];
extern char key;
extern char last_key;
extern int stars_collected;
extern int speed;
extern int x;
extern int y;
extern int ox, oy;
extern int star_x, star_y;

struct snakestail {
    int old_x, old_y;
};

extern struct snakestail snakestail_old[MAP_SNAKE_LENGTH];

#endif
