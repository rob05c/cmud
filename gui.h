#ifndef guiH
#define guiH

#include "data.h"

windows initWindows(int height, int width);
void initColors(); /* create color pairs for ncurses  */
void printGui(world* w); /* redraw the window, printing everything */
void printMap(world* w); /* used by printGui(). Draws everything in the map_list */
void printMessage(const char* msg, world* w);
void printAttackMessage(const char* msg, world* w);
void windows_refresh(windows w);

#endif
