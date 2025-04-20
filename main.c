#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <iso646.h>
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

void init_game_state() {
    stars_collected = 0;
    x = 5;
    y = 11;
    ox = x;
    oy = y;
    star_x = 8;
    star_y = 3;
    map_width = INITIAL_MAP_WIDTH;
    map_height = INITIAL_MAP_HEIGHT;
    last_key = 'd';

    for (int i = 0; i < MAP_SNAKE_LENGTH; i++) {
        snakestail_old[i].old_x = -1;
        snakestail_old[i].old_y = -1;
    }

    num_obstacles = 0;
}

void add_obstacles(int level) {
    int new_obstacles = level;
    for (int i = 0; i < new_obstacles; i++) {
        if (num_obstacles >= MAX_OBSTACLES) break;

        int ox, oy;
        do {
            ox = rand() % (map_width - 2) + 1;
            oy = rand() % (map_height - 2) + 1;
        } while (map[oy][ox] != ' ' || (ox == x && oy == y) || (ox == star_x && oy == star_y));

        // Сохраняем координаты препятсвий
        obstacles_x[num_obstacles] = ox;
        obstacles_y[num_obstacles] = oy;

        // Перемещаем препятсвие на карту
        map[oy][ox] = '#';
        num_obstacles++;
    }
}

void init_map() {
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            if (i == 0 or i == map_height - 1 or j == 0 or j == map_width - 1) {
                map[i][j] = '#';
            } else {
                map[i][j] = ' ';
            }
        }
        map[i][map_width] = '\0';
    }

    for (int i = 0; i < num_obstacles; i++) {
        map[obstacles_y[i]][obstacles_x[i]] = '#';
    }
}

void lvl(int stars_collected, char *buffer) {
    if ((stars_collected >= 0) and (stars_collected <= 3)) {
        strcpy(buffer, "1 (x1,00)\n");
    } else if ((stars_collected >= 3) and (stars_collected < 6)) {
        strcpy(buffer, "2 (x1,50)\n");
    } else if ((stars_collected >= 6) and (stars_collected < 9)) {
        strcpy(buffer, "3 (x2,00)\n");
    } else if (stars_collected >= 9) {
        strcpy(buffer, "4 (x2,50)\n");
    }
}

void increase_map_size() {
    if (map_width < MAX_MAP_WIDTH and map_height < MAX_MAP_HEIGHT) {
        map_width += 2;
        map_height += 1;
        init_map();
        add_obstacles(stars_collected / 3 + 1);
    }
}

void draw_game() {
    //move(0, 0);
    clear();
    attron(COLOR_PAIR(TEXT_COLOR));
    char level_text[10];
    mvprintw(0, 0, "Stars collected: %d\n", stars_collected);
    lvl(stars_collected, level_text);
    mvprintw(1, 0, "Speed: %s", level_text);

    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            char cell = map[i][j];
            if (cell == '#') {
                attron(COLOR_PAIR(WALL_COLOR));
                mvaddch(i + 2, j, '#');
                attroff(COLOR_PAIR(WALL_COLOR));
            } else if (cell == '*') {
                attron(COLOR_PAIR(STAR_COLOR));
                mvaddch(i + 2, j, '*');
                attroff(COLOR_PAIR(STAR_COLOR));
            } else if (cell == '>' or cell == '<' or cell == '^' or cell == 'v' or cell == '@') {
                attron(COLOR_PAIR(SNAKE_COLOR));
                mvaddch(i + 2, j, cell);
                attroff(COLOR_PAIR(SNAKE_COLOR));
            } else {
                mvaddch(i + 2, j, ' ');
            }
        }
    }
}

void update_snake_position() {
    ox = x;
    oy = y;

    if (key != ERR) {
        switch (key) {
            case 'w':
                if (last_key != 's') {
                    y--;
                    last_key = 'w';
                } else {
                    y++;
                }
                break;
            case 's':
                if (last_key != 'w') {
                    y++;
                    last_key = 's';
                } else {
                    y--;
                }
                break;
            case 'a':
                if (last_key != 'd') {
                    x--;
                    last_key = 'a';
                } else {
                    x++;
                }
                break;
            case 'd':
                if (last_key != 'a') {
                    x++;
                    last_key = 'd';
                } else {
                    x--;
                }
                break;
        }
    } else {
        switch (last_key) {
            case 'w': y--; break;
            case 's': y++; break;
            case 'a': x--; break;
            case 'd': x++; break;
        }
    }
}
bool is_collision() {
    if (y < 0 or y >= map_height or x < 0 or x >= map_width) {
        if (stars_collected == 0) {
            x = ox;
            y = oy;
            return false;
        }
        return true;
    }

    if (map[y][x] == '#') {
        if (stars_collected == 0) {
            x = ox;
            y = oy;
            return false;
        }
        return true;
    }

    for (int i = 0; i < stars_collected; i++) {
        if (x == snakestail_old[i].old_x && y == snakestail_old[i].old_y) {
            return true;
        }
    }
    return false;
}

void place_star() {
    do {
        star_x = rand() % (map_width - 2) + 1;
        star_y = rand() % (map_height - 2) + 1;
    } while (map[star_y][star_x] != ' ' || (x == star_x && y == star_y));
}

void update_game_logic() {
    update_snake_position();

    if (is_collision()) {
        move(map_height + 2, 5);
        mvprintw(map_height + 2, 5, "GAME OVER! Press R to Retry or Q to quit. Stars collected: %d", stars_collected);
        refresh();

        while (true) {
            key = getch();
            if (key == 'r' or key == 'R') {
                init_game_state();
                init_map();
                return;
            } else if (key == 'q' or key == 'Q') {
                endwin();
                exit(0);
            }
        }
    }

    // Проверка сбора зведз
    if (x == star_x and y == star_y) {
        stars_collected++;
        place_star();
        if (stars_collected % 3 == 0) {
            increase_map_size();

            int current_level = stars_collected / 3;
            add_obstacles(current_level);
        }
    }
    
    // Установка скорости игры
    if ((stars_collected >= 0) and (stars_collected < 3)) {
        speed = 10;
    } else if ((stars_collected >= 3) and (stars_collected < 6)) {
        speed = 7;
    } else if ((stars_collected >= 6) and (stars_collected < 9)) {
        speed = 5;
    } else if (stars_collected >= 9) {
        speed = 3;
    }

    // Обновление хвоста
    for (int i = stars_collected; i >= 0; i--) {
        //snakestail_old[i] = snakestail_old[i - 1];
        snakestail_old[i + 1].old_x = snakestail_old[i].old_x;
        snakestail_old[i + 1].old_y = snakestail_old[i].old_y;
    }
    snakestail_old[0].old_x = ox;
    snakestail_old[0].old_y = oy;

    // Обновление карты
    init_map();
    if (stars_collected > 0) {
        for (int i = 0; i < stars_collected; i++) {
            if (snakestail_old[i].old_x != -1 and snakestail_old[i].old_y != -1) {
                map[snakestail_old[i].old_y][snakestail_old[i].old_x] = '#';
            }
        }
    }
    switch (last_key) {
        case 'w': map[y][x] = '^'; break;
        case 's': map[y][x] = 'v'; break;
        case 'a': map[y][x] = '<'; break;
        case 'd': map[y][x] = '>'; break;
        default: map[y][x] = '@'; break;
    }

    map[star_y][star_x] = '*';
}

void game_loop() {
    srand(time(NULL));
    init_game_state();
    init_map();
    
    while (true) {
        clear();
        draw_game();
        refresh();
        
        halfdelay(speed);
        key = getch();
        // Если пользователь ввёл новую команду, обновляем направление
        if (key != ERR) {
            if (key == 'w' or key == 's' or key == 'a' or key == 'd') {
                last_key = key;
            } else if (key == 'q' or key == 'Q') {
                break;
            }
        }

        update_game_logic();
    }
}

int main() {
    initscr();
    curs_set(0);
    noecho();
    start_color();
    
    // Инициализация цветовых пар
    init_pair(WALL_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SNAKE_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(STAR_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TEXT_COLOR, COLOR_CYAN, COLOR_BLACK);

    game_loop();

    endwin();
    return 0;
}
