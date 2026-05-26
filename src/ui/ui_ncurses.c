#include <ncurses.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "model.h"
#include "backend.h"

/*
 * fc-top ui_ncurses
 */

static void draw_rounded_box(WINDOW *win) {
    int max_y, max_x;
    getmaxyx(win, max_y, max_x);

    for (int i = 1; i < max_x - 1; i++) {
        mvwaddstr(win, 0, i, "─");
        mvwaddstr(win, max_y - 1, i, "─");
    }
    for (int i = 1; i < max_y - 1; i++) {
        mvwaddstr(win, i, 0, "│");
        mvwaddstr(win, i, max_x - 1, "│");
    }

    mvwaddstr(win, 0, 0, "╭");
    mvwaddstr(win, 0, max_x - 1, "╮");
    mvwaddstr(win, max_y - 1, 0, "╰");
    mvwaddstr(win, max_y - 1, max_x - 1, "╯");
}

void init_ui(){
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    timeout(100);
    curs_set(0);
    keypad(stdscr, TRUE);
    leaveok(stdscr, TRUE);
}

void render_bar(WINDOW *win, double percentage, int bar_width, int y, int x){
    int filled_width = (int) ((percentage / 100.0) * bar_width);
    wmove(win, y, x);
    wprintw(win, "[");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled_width) {
            wprintw(win, "█");
        } else {
            wprintw(win, "-");
        }
    }
    wprintw(win, "]");
}

void render_memory(WINDOW *win, const struct Memory *memory){
    mvwprintw(win, 1, 2, "Memory Usage");
    render_bar(win, memory->UsagePercentage, 30, 2, 2);
    mvwprintw(win, 3, 2, "%.2f GiB (%.1f%%) of %.2f GiB", memory->UsedGiB, memory->UsagePercentage, memory->TotalGiB);
}

void render_swap(WINDOW *win, const struct Swap *swap){
    mvwprintw(win, 5, 2, "Swap Usage");
    render_bar(win, swap->UsagePercentage, 30, 6, 2);
    mvwprintw(win, 7, 2, "%.2f GiB (%.1f%%) of %.2f GiB", swap->UsedGiB, swap->UsagePercentage, swap->TotalGiB);
}

void render_uptime(WINDOW *win, const struct Uptime *uptime){
    mvwprintw(win, 1, 2, "System Info");
    mvwprintw(win, 3, 2, "Uptime: %d d %d h %d m %lis", uptime->days, uptime->hours, uptime->minutes, uptime->seconds);
}

void render_dashboard(const struct Memory *memory, const struct Uptime *uptime, const struct Swap *swap) {
    werase(stdscr); // Borramos el lienzo base (que ahora aloja a todos)

    int main_h = LINES;
    int main_w = COLS;

    if (main_h < 15 || main_w < 60) {
        mvprintw(0, 0, "Very small terminal...");
        refresh();
        return;
    }

    WINDOW *win_main = subwin(stdscr, main_h, main_w, 0, 0);
    draw_rounded_box(win_main);
    mvwprintw(win_main, 0, 2, " fc-top %s ", FCTOP_VERSION);

    time_t raw_time;
    struct tm *time_info;
    char time_buffer[40];

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_buffer, sizeof(time_buffer), " %Y-%m-%d %H:%M:%S ", time_info);
    int time_len = strlen(time_buffer);

    if (main_w > time_len + 25) {
        mvwprintw(win_main, 0, main_w - time_len - 2, "%s", time_buffer);
    }

    int sub_h = (main_h - 2) / 2;
    int sub_w = (main_w - 4) / 2;

    WINDOW *win_ram = subwin(stdscr, sub_h, sub_w, 1, 2);
    draw_rounded_box(win_ram);
    render_memory(win_ram, memory);
    render_swap(win_ram,swap);

    WINDOW *win_sys = subwin(stdscr, sub_h, sub_w, 1, 2 + sub_w);
    draw_rounded_box(win_sys);
    render_uptime(win_sys, uptime);

    int bot_h = main_h - sub_h - 3;
    WINDOW *win_proc = subwin(stdscr, bot_h, main_w - 4, 1 + sub_h, 2);
    draw_rounded_box(win_proc);
    mvwprintw(win_proc, 1, 2, "PROCESSES");
    mvwprintw(win_proc, bot_h - 2, 2, "Press 'q' to quit");

    touchwin(stdscr);

    wnoutrefresh(stdscr);
    doupdate();

    delwin(win_ram);
    delwin(win_sys);
    delwin(win_proc);
    delwin(win_main);
}

void end_ui(){
    endwin();
}
