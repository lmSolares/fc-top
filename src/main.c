#include <ncurses.h>
#include "backend.h"
#include "model.h"
#include <signal.h>

int main(){

    struct Memory memory;
    struct Swap swap;
    struct Uptime uptime;
    struct ProcessList process_list = {NULL, 0, 0};

    int selected_process = 0;
    int scroll_offset = 0;

    init_ui();

    while(1){

        // Get data
        get_memory(&memory, &swap);
        get_uptime(&uptime);
        get_processes(&process_list);

        if (process_list.count > 0 && selected_process >= process_list.count) {
            selected_process = process_list.count - 1;
        } else if (process_list.count == 0) {
            selected_process = 0;
        }

        // Render dashboard
        render_dashboard(&memory, &uptime, &swap, &process_list, selected_process, &scroll_offset);

        // Quit
        int in = getch();
        if(in == 'q'){
            break;
        }

        // UP/DOWN & Kill
        else if (in == KEY_UP) {
            if (selected_process > 0) {
                selected_process--;
            }
        }
        else if (in == KEY_DOWN) {
            if (selected_process < process_list.count - 1) {
                selected_process++;
            }
        }
        else if (in == 'k' || in == 'K') {
            if (process_list.count > 0 && selected_process < process_list.count) {
                struct Process *p = &process_list.processes[selected_process];

                if (confirm_kill(p->pid, p->name)) {
                    kill(p->pid, SIGTERM);
                }
            }
        }

    }

    free_processes(&process_list);
    end_ui();

    return 0;
}
