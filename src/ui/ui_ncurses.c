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
    
    int bar_width = 50;
    int filled_width = (int)((memory->UsagePercentage / 100.0) * bar_width);
    
    // Draw memory bar
    printw("[");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_width) {
            printw("#%");
        } else {
            printw("-");
        }
    }
    printw("]\n%.2f GiB (%.1f\%) of %.2f GiB\n", memory->UsedGiB, memory->UsagePercentage, memory->TotalGiB);

    refresh();
    return;
}

void end_ui(){
    endwin();
    return;
}