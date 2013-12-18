#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include "data.h"
#include "gui.h"
#include "input.h"

/* 
This is really a debug function. 
In the final game, world data will be read from the database, 
or created dynamically.
*/
map_list createMap() {
  map_list list = map_create(0);
  list = map_add(list, 2, 3, '&', 1);
  list = map_add(list, 3, 3, '#', 2);
  list = map_add(list, 4, 4, '+', 3);
  list = map_add(list, 33, 24, '$', 4);
  list = map_add(list, 1, 2, '@', 5);
  return list;
}

void init(world* w) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
  start_color(); 
  initColors();
  getmaxyx(stdscr, w->WindowHeight, w->WindowWidth);
  --w->WindowHeight;
  w->Map = createMap();
  w->Location = point_create(0, 0);
  w->Color = 4;
  w->Symbol = "☺"; 
  w->Windows = initWindows(w->WindowHeight, w->WindowWidth);

  char buffer[50];
  sprintf(buffer, "points: %d", (int)*w->Map.Length); /* debug (mostly) */
  curs_set(0); /* hide cursor */ 
  move(0,0);
  mousemask(BUTTON1_PRESSED, NULL);

  refresh();
}


int main(int argc, char** argv) {
  world w;
  int done;
  init(&w);

  done = 0;

  printMap(&w); 
  world_refresh(&w);

  handleInput(&w, &done);

  endwin();
  return 0;
}
