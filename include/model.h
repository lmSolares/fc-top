#ifndef MODEL_H
#define MODEL_H
#include "backend.h"

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
void render_memory(const struct Memory *memory);
void render_uptime(const struct Uptime *uptime);
void render_bar(double percentage, int bar_width);

#endif
