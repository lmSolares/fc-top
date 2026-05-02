#include <ncurses.h>
#include "model.h"

/*
* fc-top
*/

void init_ui(){
    initscr();
    cbreak();
    noecho();
    timeout(100);
    curs_set(0);
    return;
}

void render_ui(const struct Memory *memory){
    erase();
    printw("fc-top v%s\n\nMemory\n", FCTOP_VERSION);
    draw_bar(memory->UsagePercentage, 50);
    printw("%.2f GiB (%.1f\%) of %.2f GiB\n", memory->UsedGiB, memory->UsagePercentage, memory->TotalGiB);
    refresh();
    return;
}

void draw_bar(double percentage, int bar_width){
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