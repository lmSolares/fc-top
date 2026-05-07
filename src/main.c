#include <ncurses.h>
#include "backend.h"
#include "model.h"

int main(){

    struct Memory memory;
    struct Uptime uptime;

    init_ui();

    while(1){
        erase();
        get_memory(&memory);
        render_memory(&memory);
        int in = getch();
        if(in == 'q'){
            break;
        }
        refresh();
    }

    end_ui();

    return 0;
}