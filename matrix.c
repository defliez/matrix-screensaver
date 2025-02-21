#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t lock;

void* printRandomNumber(void* arg) {
    int xCoord = *(int*)arg;
    free(arg);
    int randomNum;
    int height, width;
    getmaxyx(stdscr, height, width);

    int y = 0;

    int randomOffset = rand() % 10;
    usleep(randomOffset * 500000);

    while (1) {
        randomNum = rand() % 10;

        pthread_mutex_lock(&lock);
        mvprintw(y, xCoord, "%d", randomNum);
        refresh();
        pthread_mutex_unlock(&lock);

        usleep(50000);
        y++;

        if (y >= height) {
            y = 0;
        }
    }
    return NULL;
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    pthread_mutex_init(&lock, NULL);

    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    attron(A_BOLD);

    pthread_t threads[140];

    for (int i = 0; i < 140; i++) {
        int* xCoord = malloc(sizeof(int));
        *xCoord = i * 3;
        pthread_create(&threads[i], NULL, printRandomNumber, (void*)xCoord);
    }

    // for (int i = 0; i < 140; i++) {
    //     pthread_join(threads[i], NULL);
    // }

    while (1) {
        usleep(100000);
    }

    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);

    getch();
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));
    endwin();

    return 0;
}
