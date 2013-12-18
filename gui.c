/* ncurses gui drawing functions */
#include <ncurses.h>
#include <string.h>
#include "gui.h"
#include "data.h"

void initColors() {
  init_color(COLOR_BLACK, 255, 0, 0);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_BLUE, COLOR_BLACK);
  init_pair(7, COLOR_WHITE, COLOR_BLACK);
}

void printMap(world* w) {
  int i;
  int end;
  map_object* o;
  map_list localMap = map_objects_in_box(w->Map, 
					 w->Location.Y - w->WindowHeight / 2, 
					 w->Location.X - w->WindowWidth / 2,
					 w->Location.Y + w->WindowHeight / 2,
					 w->Location.X + w->WindowWidth / 2);
  for(i = 0, end = *localMap.Length; i != end; ++i) {
    o = &localMap.Objects[i];
    wattron(w->Windows.Main, COLOR_PAIR(o->Color)); 
    mvwaddch(w->Windows.Main, o->Point.Y, o->Point.X, o->Symbol);
    wattroff(w->Windows.Main, COLOR_PAIR(o->Color));
  }
  
  wattron(w->Windows.Main, COLOR_PAIR(w->Color)); 
  mvwprintw(w->Windows.Main, w->Location.Y, w->Location.X, w->Symbol);
  wattroff(w->Windows.Main, COLOR_PAIR(w->Color));

}

/* prints basic GUI that should always be visible */
void printGui(const char* msg, world* w) {
/*  box(stdscr, 0, 0); */
  mvwprintw(w->Windows.Main, w->WindowHeight, w->WindowWidth / 2 - strlen(msg) / 2, msg);
  mvwprintw(w->Windows.Main, w->WindowHeight, 1,"%s", "Quadtree demo ");
  const char* msgExit = " Press 'q' to exit";
  mvwprintw(w->Windows.Main, w->WindowHeight, w->WindowWidth - strlen(msgExit) - 1, msgExit);

  printMap(w);
}


WINDOW* initMainWindow(int height, int width, int minusHeight) {
  return newwin(height - minusHeight, width, 0, 0);
}

WINDOW* initRoomStatus(int myHeight, int height, int width) {

  return newwin(myHeight, width, height - myHeight, 0);
}

windows initWindows(int height, int width) {
  const int STATUS_HEIGHT = 10;
  windows ws;
  ws.Status = initRoomStatus(height, width, STATUS_HEIGHT);
  ws.Main = initMainWindow(height, width, STATUS_HEIGHT);
  return ws;
}

void windows_refresh(windows w) {
  wrefresh(w.Main);
  wrefresh(w.Status);
}
