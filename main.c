#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>
#include <math.h>

#include "map.h"
#include "walls.h"

#define deg_to_rad(a) ((a) * M_PI / 180.0f)

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

bool should_exit = false;

float p_x = 3;
float p_y = 3;
float p_r = 0;

float fov = deg_to_rad(45.0f);

char *map;

void draw_border(int width, int height)
{
    // corners
    mvprintw(0, 0, "+");
    mvprintw(height-1, 0, "+");
    mvprintw(0, width-1, "+");
    mvprintw(height-1, width-1, "+");

    // sides
    for (int i = 1; i < height-1; i++) {
        mvprintw(i, 0, "|");
        mvprintw(i, width-1, "|");
    }

    for (int i = 1; i < width-1; i++) {
        mvprintw(0, i, "-");
        mvprintw(height-1, i, "-");
    }
}

void render()
{
    clear();

    int width, height;
    getmaxyx(stdscr, height, width);

    for (int x = 0; x < width; x++) {
        float ray_angle = (p_r - (fov / 2.0f)) + ((float)x / width) * fov;

        float ray_x = sinf(ray_angle);
        float ray_y = cosf(ray_angle);
        float ray_dist = 0;
        bool hit = false;

        while (!hit) {
            ray_dist += 0.1f;

            int cur_x = (int) (p_x + ray_x * ray_dist);
            int cur_y = (int) (p_y + ray_y * ray_dist);

            if (cur_x < 0 || cur_x > 15 || cur_y < 0 || cur_y > 15) {
                hit = true;
            } else {
                if (map[cur_x + cur_y * MAP_WIDTH] == '#') {
                    hit = true;
                }
            }
        }

        int ceiling = (float)(height / 2.0f) - (height / ray_dist);
        int floor = height - ceiling;

        for (int y = 0; y < height; y++) {
            if (y > ceiling && y < floor) {
                char *wall = " ";
                if (ray_dist < 3)
                    wall = W_FULL;
                else if (ray_dist >= 3 && ray_dist < 8)
                    wall = W_MEDIUM;
                else
                    wall = W_LIGHT;
                mvprintw(y, x, wall);
            } else if (y > floor) {
                float d = 1.0f - ((y - height / 2.0f) / (height / 2.0f));
                char *f;
                if (d < 0.25f)
                    f = "#";
                else if (d < 0.5f)
                    f = "+";
                else if (d < 0.75f)
                    f = "-";
                else
                    f = ".";
                mvprintw(y, x, f);
            }
        }
    }

    draw_border(width, height);

    refresh();
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    map = read_map(argv[1], MAP_WIDTH, MAP_HEIGHT);

    while (!should_exit) {
        int c = getch();

        if (c == 27)
            should_exit = true;

        switch(c) {
            case KEY_UP:
                p_x += sinf(p_r) / 2.0f;
                p_y += cosf(p_r) / 2.0f;
                break;
            case KEY_DOWN:
                p_x -= sinf(p_r) / 2.0f;
                p_y -= cosf(p_r) / 2.0f;
                break;
            case KEY_RIGHT:
                p_r += 0.1;
                break;
            case KEY_LEFT:
                p_r -= 0.1;
                break;
        }

        render();
    }

    endwin();

    exit(EXIT_SUCCESS);
}
