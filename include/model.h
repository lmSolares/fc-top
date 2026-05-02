#ifndef MODEL_H
#define MODEL_H
#include "backend.h"

/*
- fc-top tui.h
*/

void init_ui();
void render_ui(const struct Memory *memory);
void end_ui();
void draw_bar(double percentage, int bar_width);

#endif