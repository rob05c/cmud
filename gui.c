/* ncurses gui drawing functions */
#include <ncurses.h>
#include <string.h>
#include <stdio.h>
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
  player* p = &w->Player;
  map_list localMap = map_objects_in_box(&w->Map, 
					 p->Location.Y - w->WindowHeight / 2, 
					 p->Location.X - w->WindowWidth / 2,
					 p->Location.Y + w->WindowHeight / 2,
					 p->Location.X + w->WindowWidth / 2);
  for(i = 0, end = *localMap.Length; i != end; ++i) {
    o = &localMap.Objects[i];
    wattron(w->Windows.Main, COLOR_PAIR(o->Color)); 
    mvwaddch(w->Windows.Main, o->Point.Y, o->Point.X, o->Symbol);
    wattroff(w->Windows.Main, COLOR_PAIR(o->Color));
  }
  
  wattron(w->Windows.Main, COLOR_PAIR(p->Color)); 
  mvwprintw(w->Windows.Main, p->Location.Y, p->Location.X, p->Symbol);
  wattroff(w->Windows.Main, COLOR_PAIR(p->Color));

}

void printObjects(world* w) {
  const char* blank = "                                                           ";
  map_object* o = map_get(&w->Map, w->Player.Location);
  npc* n = NULL;
  int color;
  float hPercent;
  mvwprintw(w->Windows.Status, 2, 2, blank);

  if(o != NULL)
    n = npc_get(&w->Npcs, o->Id);
  if(n == NULL) {
    mvwprintw(w->Windows.Status, 2, 2, "Nothing is here.");
    return;
  }


  hPercent = ((float)n->Health) / n->MaxHealth;
  if(hPercent > 0.66)
    color = 3;
  else if(hPercent > 0.33)
    color = 4;
  else
    color = 1;
  wattron(w->Windows.Status, COLOR_PAIR(color));
  mvwaddch(w->Windows.Status, 2, 2, n->MapObject.Symbol);
  wattroff(w->Windows.Status, COLOR_PAIR(color));
  mvwprintw(w->Windows.Status, 2, 4, n->Desc);
}

static void printPlayerStatusHealth(world* w) {
  char healthLine[20];
  char healthString[20];
  char maxHealthString[20];
  int x;
  int color;
  float healthPercent;
  const int HEALTH_Y = 2;
  player* p = &w->Player;

  strcpy(healthLine, "Health: ");
  sprintf(healthString, "%d", p->Health);
  sprintf(maxHealthString, "%d", p->MaxHealth);

  healthPercent = ((float)p->Health) / p->MaxHealth;
  if(healthPercent > 0.66)
    color = 3;
  else if(healthPercent > 0.33)
    color = 4;
  else
    color = 1;

  x = w->WindowWidth - 20;
  mvwprintw(w->Windows.Status, HEALTH_Y, x, "                   ");
  mvwprintw(w->Windows.Status, HEALTH_Y, x, healthLine);
  x += strlen(healthLine);
  wattron(w->Windows.Status, COLOR_PAIR(color));
  mvwprintw(w->Windows.Status, HEALTH_Y, x, healthString);
  x += strlen(healthString);
  mvwprintw(w->Windows.Status, HEALTH_Y, x, "/");
  x += 1;
  mvwprintw(w->Windows.Status, HEALTH_Y, x, maxHealthString);
  wattroff(w->Windows.Status, COLOR_PAIR(color));
}

static void printPlayerStatusExp(world* w) {
  const int EXP_Y = 3;
  const int LEVEL_Y = 4;
  char expString[20];
  char levelString[20];
  int x;
  int color;
  player* p = &w->Player;
  const char* EXP_STRING = "Experience: ";
  const char* LEVEL_STRING = "Level: ";

  x = w->WindowWidth - 20;
  color = 2;
  sprintf(expString, "%lu", p->Experience);
  mvwprintw(w->Windows.Status, EXP_Y, x, "                  ");
  wattron(w->Windows.Status, COLOR_PAIR(color));
  mvwprintw(w->Windows.Status, EXP_Y, x, EXP_STRING);
  x += strlen(EXP_STRING);
  mvwprintw(w->Windows.Status, EXP_Y, x, expString);
  wattroff(w->Windows.Status, COLOR_PAIR(color));

  x = w->WindowWidth - 20;
  color = 3;
  sprintf(levelString, "%d", player_level(p));
  mvwprintw(w->Windows.Status, LEVEL_Y, x, "                  ");
  wattron(w->Windows.Status, COLOR_PAIR(color));
  mvwprintw(w->Windows.Status, LEVEL_Y, x, LEVEL_STRING);
  x += strlen(LEVEL_STRING);
  mvwprintw(w->Windows.Status, LEVEL_Y, x, levelString);
  wattroff(w->Windows.Status, COLOR_PAIR(color));
}

void printPlayerStatus(world* w) {
  printPlayerStatusHealth(w);
  printPlayerStatusExp(w);
}

/* @todo add room message window */
void printMessage(const char* msg, world* w) {
  const char* blank = "                                                                                                    ";
  mvwprintw(w->Windows.Status, 7, 2, blank);
  mvwprintw(w->Windows.Status, 7, 2, msg);
}

void printAttackMessage(const char* msg, world* w) {
  const char* blank = "                                                                                                    ";
  mvwprintw(w->Windows.Status, 5, 2, blank);
  mvwprintw(w->Windows.Status, 5, 2, msg);
}

/* prints basic GUI that should always be visible */
void printGui(world* w) {
  wborder(w->Windows.Main, '*', '*', '*', '*', '*', '*', '*', '*'); 
  wborder(w->Windows.Status, '#', '#', '#', '#', '#', '#', '#', '#'); 

  printObjects(w);
  printPlayerStatus(w);
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
  ws.Status = initRoomStatus(STATUS_HEIGHT, height, width);
  ws.Main = initMainWindow(height, width, STATUS_HEIGHT);
  return ws;
}

void windows_refresh(windows w) {
  wrefresh(w.Main);
  wrefresh(w.Status);
}
