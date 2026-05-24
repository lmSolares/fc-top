#include <ncurses.h>
#include "model.h"
#include "backend.h"

/*
* fc-top ui_ncurses
*/

void init_ui(){
    initscr();
    cbreak();
    noecho();
    timeout(100);
    curs_set(0);
    return;
}

void render_memory(const struct Memory *memory){
    render_bar(memory->UsagePercentage, 50);
    printw("Memory\n%.2f GiB (%.1f\%) of %.2f GiB\n", memory->UsedGiB, memory->UsagePercentage, memory->TotalGiB);
}

void render_uptime(const struct Uptime *uptime){
    printw("Uptime: %d d %d h %d m %lis\n",uptime->days,uptime->hours, uptime->minutes, uptime->seconds);
}

void render_bar(double percentage, int bar_width){
    int filled_width = (int) ((percentage / 100.0) * bar_width);
    printw("[");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_width) {
            printw("*%");
        } else {
            printw("-");
        }
    }
    printw("]\n");
}

void end_ui(){
    endwin();
    return;
}
