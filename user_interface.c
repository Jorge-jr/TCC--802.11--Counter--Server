#include "user_interface.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "device_handler.h"

/*
Using NCURSES under XTERM
A resize operation in X sends SIGWINCH to the application running under xterm. The easiest way to handle SIGWINCH is to do an endwin, followed by an refresh and a screen repaint you code yourself. The refresh will pick up the new screen size from the xterm's environment.

That is the standard way, of course (it even works with some vendor's curses implementations). Its drawback is that it clears the screen to reinitialize the display, and does not resize subwindows which must be shrunk. Ncurses provides an extension which works better, the resizeterm function. That function ensures that all windows are limited to the new screen dimensions, and pads stdscr with blanks if the screen is larger.

The ncurses library provides a SIGWINCH signal handler, which pushes a KEY_RESIZE via the wgetch() calls. When ncurses returns that code, it calls resizeterm to update the size of the standard screen's window, repainting that (filling with blanks or truncating as needed). It also resizes other windows, but its effect may be less satisfactory because it cannot know how you want the screen re-painted. You will usually have to write special-purpose code to handle KEY_RESIZE yourself.

*/



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
    mvwprintw(center, 2, 4, "Nº \t Address \t Last detected \t     Count");
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

void ui_clear(){
    clearok(top, 1);
}

