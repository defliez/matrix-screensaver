#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_mutex_t lock;
int screenFilledThreads;

void* printRandomNumber(void* arg) {
    int xCoord = *(int*)arg;
    free(arg);
    int randomNum;
    int height, width;

    int y = 0;

    int randomOffset = rand() % 10;
    usleep(randomOffset * 500000);

    while (1) {
    getmaxyx(stdscr, height, width);
        randomNum = rand() % 10;

        pthread_mutex_lock(&lock);
        mvprintw(y, xCoord, "%d", randomNum);
        refresh();
        pthread_mutex_unlock(&lock);

        usleep(50000);
        y++;

        if (y >= height) {
            y = 0;
            screenFilledThreads++;
        }
    }
    return NULL;
}



void* printWakeUp(void* arg) {
    int height, width;
    getmaxyx(stdscr, height, width);
    int centerY = height / 2;
    int centerX = width / 2 - (strlen("WAKE UP") / 2);

    mvprintw(centerY, centerX, "WAKE UP");
    return NULL;
}

int main() {
    screenFilledThreads = 0;

    initscr();
    noecho();
    curs_set(0);
    pthread_mutex_init(&lock, NULL);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_GREEN, -1);
    attron(COLOR_PAIR(1));
    attron(A_BOLD);

    sleep(1);

    int width, height;

    int max_attempts = 50;
    for (int i = 0; i < max_attempts; i++) {
        clear();
        refresh();
        getmaxyx(stdscr, height, width);
        if (width > 80) {
            break;
        }
        usleep(100000);
    }

    int threadCnt = width / 2;

    pthread_t threads[threadCnt];

    for (int i = 0; i < threadCnt; i++) {
        int* xCoord = malloc(sizeof(int));
        *xCoord = i * 2;
        pthread_create(&threads[i], NULL, printRandomNumber, (void*)xCoord);
    }

    pthread_t threadWakeUp;

    while (1) {
        usleep(100000);
        if (screenFilledThreads >= threadCnt) {
            pthread_create(&threadWakeUp, NULL, printWakeUp, NULL);
        }
    }

    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);

    getch();
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));
    endwin();

    return 0;
}
