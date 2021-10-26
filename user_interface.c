#include "user_interface.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "device_handler.h"


WINDOW *top,*center, *bottom;
int mx=0, my=0;


void ui_start(){

    //Device** device_list = get_device_list();

    initscr ();
    getmaxyx(stdscr, mx, my);
    top = newwin(4, my, 0, 0);
    center = newwin(mx-7, my, 4, 0);
    bottom = newwin(3, my, mx-3, 0);

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN,   COLOR_BLUE);
        init_pair(2, COLOR_BLACK,   COLOR_CYAN);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_BLACK,   COLOR_WHITE);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }
    refresh();
    box(bottom, 0, 0);
    box(top, 0, 0);
    box(center, 0, 0);
    box(stdscr, 0, 0);
    curs_set (0);  //Hide cursor
    wattrset(top, COLOR_PAIR(1));
    wbkgd(bottom, COLOR_PAIR(5));
    mvwprintw(top, 1, 1, "Found devices: %d", 0);
    mvwprintw(center, 2, 5, "Nº \t Address \t Last detected \t Count");
    refresh();
    wrefresh(top);
    wrefresh(center);
    wrefresh(bottom);
}

void ui_print(int window, int line, int column, char* content)
{
    if (window == 1){                             // 1 = top window
        mvwprintw(top, line, column, "%s", content);
        wrefresh(top);
    }else if (window == 2)  {                     // 2 = center window
        mvwprintw(center, line, column, "%s", content);
        wrefresh(center);
    }else{                                        // 3 = bottom window
        mvwprintw(bottom, line, column, "%s", content);
        wrefresh(bottom);
    }
    refresh();
}


