#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

static void finish(int sig);


int main(int argc, char *argv[])
{
    int num = 0;

    (void) signal(SIGINT, finish);

    (void) initscr();
    keypad(stdscr, TRUE);
    (void) nonl();
    (void) cbreak();

    if (has_colors())
    {
        start_color();
        init_pair(1, COLOR_GREEN,   COLOR_BLUE);
        init_pair(2, COLOR_GREEN,   COLOR_BLACK);
        init_pair(3, COLOR_YELLOW,  COLOR_BLACK);
        init_pair(4, COLOR_BLUE,    COLOR_BLACK);
        init_pair(5, COLOR_BLACK,   COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE,   COLOR_BLACK);
    }
    for (;;)
    {
        //int c = getch();
        attrset(COLOR_PAIR(1));
        printw("Stein\n");
        attrset(COLOR_PAIR(3));
        printw("Stein");
        refresh();
        move(0, 0);
        sleep(10);
        clearok(stdscr, true);
        refresh();
        attrset(COLOR_PAIR(6));
        printw("Vengastyein");

        //attrset(COLOR_PAIR(num % 8));
        //num++;
    }
    finish(0);
}

static void finish(int sig)
{
    endwin();

    exit(0);
}


