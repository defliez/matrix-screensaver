#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>
#include <unistd.h>

pthread_mutex_t lock;
int screenFilledThreads;

void* printRandomNumber(void* arg) {
    int xCoord = *(int*)arg;
    free(arg);
    int randomNum;
    int height, width;

    int randomOffset = rand() % 10;
    usleep(randomOffset * 500000);

    int length = rand() % 50;

    bool finishedOnce = false;

    while (1) {
        int j = 0;
        getmaxyx(stdscr, height, width);
        for (int i = 0; i < height + length; i++) {
            randomNum = rand() % 2;

            pthread_mutex_lock(&lock);
            mvprintw(i, xCoord, "%d", randomNum);
            refresh();
            pthread_mutex_unlock(&lock);

            if (i >= length) {
                pthread_mutex_lock(&lock);
                mvprintw(j, xCoord, " ");
                refresh();
                pthread_mutex_unlock(&lock);

                j++;
            }

            if (i >= height) {
                i = 0;
                finishedOnce = true;
            }

            if (j >= height) {
                j = 0;
            }
            usleep(50000);
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

    int width = getmaxx(stdscr);
    int maxThreads = 100;
    int threadCnt = width / 2;
    if (threadCnt > maxThreads) {
        threadCnt = maxThreads;
    }

    pthread_t threads[threadCnt];

    for (int i = 0; i < threadCnt; i++) {
        int* xCoord = malloc(sizeof(int));
        *xCoord = i * 3;
        pthread_create(&threads[i], NULL, printRandomNumber, (void*)xCoord);
    }

    // pthread_t threadWakeUp;
    //
    // while (1) {
    //     usleep(100000);
    //     if (screenFilledThreads >= threadCnt - 1) {
    //         pthread_create(&threadWakeUp, NULL, printWakeUp, NULL);
    //     }
    // }

    for (int i = 0; i < threadCnt; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_exit(NULL);
    pthread_mutex_destroy(&lock);

    getch();
    attroff(A_BOLD);
    attroff(COLOR_PAIR(1));
    endwin();

    return 0;
}

