#include "cafe.h"

#include "tea.h"
#include "mocha.h"


/************************
 * Window
 ************************/

int cafe_window_flags() { return tea_window_flags(); }

int cafe_window_title(const char *title, char *out) { return tea_window_title(title, out); }
int cafe_window_pos(cf_Point *out, int x, int y) { return tea_window_pos((te_point_t*)out, x, y); }
int cafe_window_size(cf_Point *out, int w, int h) { return tea_window_size((te_point_t*)out, w, h); }
int cafe_window_minsize(cf_Point *out, int w, int h) { return tea_window_minsize((te_point_t*)out, w, h); }
int cafe_window_maxsize(cf_Point *out, int w, int h) { return tea_window_maxsize((te_point_t*)out, w, h); }

int cafe_window_minimize() { return tea_window_minimize(); }
int cafe_window_maximize() { return tea_window_maximize(); }
int cafe_window_restore() { return tea_window_restore(); }
int cafe_window_fullscreen(int mode) { return tea_window_fullscreen(mode); }
int cafe_window_bordered(int bordered) { return tea_window_bordered(bordered); }
