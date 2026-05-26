#include <ncurses.h>
#include "backend.h"
#include "model.h"

int main(){

    struct Memory memory;
    struct Swap swap;
    struct Uptime uptime;

    init_ui();

    while(1){

        // Get data
        get_memory(&memory, &swap);
        get_uptime(&uptime);

        render_dashboard(&memory, &uptime, &swap);

        int in = getch();
        if(in == 'q'){
            break;
        }
    }

    end_ui();

    return 0;
}
