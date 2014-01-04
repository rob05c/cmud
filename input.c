#include "input.h"
#include <time.h>
#include <ncurses.h>
#include "gui.h"
#include "string.h"

void handleInput(world* w, int* done) {
/*  clock_t cl;*/
  int sec;
  int last_sec;
  char debugC[50];

  last_sec = 0;

  while(!*done) {
    const int c = getch();
    if(c != ERR) {
      if(c == KEY_MOUSE)
	handleMouse(w);
      else if(c == 'q') {
	break;
      }
      else
	handleKey(c, w);
    }

    sec = clock() / CLOCKS_PER_SEC;
    sprintf(debugC, "%d", sec);
    mvwprintw(w->Windows.Status, 6, 5, debugC); /* debug */
    windows_refresh(w->Windows);
    if(last_sec != sec && sec % 2 == 0) {
      tick(w);
    }
    last_sec = sec;
  }
}

void handleMouse(world* w) {
  MEVENT e;
  UNUSED(w);

  getmouse(&e);
/*  insertPoint(e.y, e.x, q); */
/*  refresh(); */
}

void handleKey(int c, world* w) {
/*
  int x;
  int y;
  getyx(stdscr, y, x);
  else if(c == ' ') {
    mvaddch(y, x, '.');
    move(y, x);
  }
*/
  mvwaddch(w->Windows.Status, 5, 5, c); /* debug */
  switch(c) {
  case KEY_UP:
    player_move(north, w);
    break;
  case KEY_DOWN:
    player_move(south, w);
    break;
  case KEY_RIGHT:
    player_move(east, w);
    break;
  case KEY_LEFT:
    player_move(west, w);
    break;
  case KEY_PPAGE:
    player_move(east, w);
    player_move(north, w);
    break;
  case KEY_NPAGE:
    player_move(east, w);
    player_move(south, w);
    break;
  case KEY_HOME:
    player_move(west, w);
    player_move(north, w);
    break;  
  case KEY_END:
    player_move(west, w);
    player_move(south, w);
    break;
  case 'k':
    player_attack(w);
    break;
  default:
    return;
  }

  printGui(w);
  world_refresh(w);
}

void player_move(direction d, world* w) {
  player* p = &w->Player;
  mvwaddch(w->Windows.Main, p->Location.Y, p->Location.X, ' ');
  switch(d) {
  case north:
    --p->Location.Y;
    break;
  case east:
    ++p->Location.X;
    break;
  case south:
    ++p->Location.Y;
    break;
  case west:
    --p->Location.X;    
    break;
  default:
    return; /* should never happen */
  }
}

void player_attack(world* w) {
  const char* attackMsg = "You hack into "; 
  map_object* o = map_get(&w->Map, w->Player.Location);
  npc* n = NULL;
  int xpos = 2;
  const int ypos = 5;

  if(o != NULL)
    n = npc_get(&w->Npcs, o->Id);
  if(n == NULL) {
    mvwprintw(w->Windows.Status, 5, 2, "You slash aimlessly at the air.");
    return;
  }

  mvwprintw(w->Windows.Status, ypos, xpos, attackMsg);
  xpos += strlen(attackMsg);
  mvwprintw(w->Windows.Status, ypos, xpos, n->Desc);
  xpos += strlen(n->Desc);
  mvwprintw(w->Windows.Status, ypos, xpos, ".");
  n->Health -= 10;
}
