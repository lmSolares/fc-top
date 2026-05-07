#ifndef MODEL_H
#define MODEL_H
#include "backend.h"

/*
- fc-top model.h
*/

// Start/end ncurses

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
void render_bar(double percentage, int bar_width);
void render_uptime();

#endif