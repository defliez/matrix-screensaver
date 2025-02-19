#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    initscr();

    start_color();
    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(1));

    int randomNum;

    while (TRUE) {
        randomNum = rand() % 10;
        printw("%d\n", randomNum);
        refresh();

        usleep(100000);
    }

    getch();

    attroff(COLOR_PAIR(1));
    endwin();

    return 0;
}
