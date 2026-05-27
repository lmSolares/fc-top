#ifndef MODEL_H
#define MODEL_H
#include "backend.h"
#include <ncurses.h>

/*
- fc-top model.h
*/


/**
 * @brief Function that initializes ncurses.
 * 
 * @return void
 */
void init_ui();

/**
 * @brief Function that terminates ncurses.
 * 
 * @return void
 */
void end_ui();

// Renders
void render_dashboard(const struct Memory *memory, const struct Uptime *uptime, const struct Swap *swap, const struct ProcessList *plist, int selected, int *scroll);
void render_memory(WINDOW *win, const struct Memory *memory);
void render_uptime(WINDOW *win, const struct Uptime *uptime);
void render_swap(WINDOW *win, const struct Swap *swap);
void render_bar(WINDOW *win, double percentage, int bar_width, int y, int x);
void render_processes(WINDOW *win, const struct ProcessList *plist, int selected, int *scroll);
int confirm_kill(int pid, const char *name);

#endif
