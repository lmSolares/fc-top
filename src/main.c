#include <ncurses.h>
#include "backend.h"
#include "model.h"

int main(){

    struct Memory memory;

    init_ui();

    while(1){
        get_memory(&memory);
        render_ui(&memory);
        int in = getch();
        if(in == 'q'){
            break;
        }
    }

    end_ui();

    return 0;
}