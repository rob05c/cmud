#include <string.h>
#include <ncurses.h>
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
/*
  list = map_add(list, 2, 3, '&', 1);
  list = map_add(list, 3, 3, '#', 2);
  list = map_add(list, 4, 4, '+', 3);
  list = map_add(list, 33, 24, '$', 4);
  list = map_add(list, 1, 2, '@', 5);
*/
  return list;
}

npc_list createNpcs(map_list mapList) {
  npc_list list = npc_list_create(0, mapList);
  list = npc_add(list, 10, 16, 'G', 1, "goblin", "A rather ugly goblin is standing here.", 100, 1);
  list = npc_add(list, 30, 6, 'G', 1, "goblin", "A rather ugly goblin is standing here.", 100, true);
  list = npc_add(list, 9, 9, 'G', 1, "goblin", "A rather ugly goblin is standing here.", 100, true);
  list = npc_add(list, 12, 42, 'G', 1, "goblin", "A rather ugly goblin is standing here.", 100, true);
  list = npc_add(list, 21, 31, 'B', 2, "baker", "A rather ugly baker is standing here, baking", 500, false);
  list = npc_add(list, 19, 7, 'B', 2, "candlestickmaker", "A portly candlestickmaker potters here, looking bemused.", 600, false);
  return list;
}

void init(world* w) {
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  timeout(0); /* no input timeout (e.g. getch) */
/*  raw(); */
  keypad(stdscr, true);
  start_color(); 
  initColors();
  getmaxyx(stdscr, w->WindowHeight, w->WindowWidth);
  --w->WindowHeight;
  w->Map = createMap();
  w->Npcs = createNpcs(createMap());
  w->Map = w->Npcs.MapList;

  w->Player.Location = point_create(0, 0);
  w->Player.Color = 3;
/*  w->Player.Symbol = "☺";  */
  w->Player.Symbol = "@"; 
  w->Player.Health = 500;
  w->Player.MaxHealth = 500;

  w->Windows = initWindows(w->WindowHeight, w->WindowWidth);

  char buffer[50];
  sprintf(buffer, "points: %d", (int)*w->Map.Length); /* debug (mostly) */
  curs_set(0); /* hide cursor */ 
  move(0,0);
  mousemask(BUTTON1_PRESSED, NULL);

  refresh();
}


/*int main(int argc, char** argv) { */
int main() {
  world w;
  int done;
  init(&w);

  done = 0;

  printGui(&w); 
  world_refresh(&w);

/*  wborder(w.Windows.Main, '*', '*', '*', '*', '*', '*', '*', '*');*/
  wborder(w.Windows.Status, '#', '#', '#', '#', '#', '#', '#', '#');
/*  wrefresh(w.Windows.Main);*/
  wrefresh(w.Windows.Status);


  handleInput(&w, &done);

  endwin();
  return 0;
}
