#include "input.h"
#include <ncurses.h>
#include "gui.h"

void handleInput(world* w, int* done) {
  while(!*done) {
    const int c = getch();
    if(c == KEY_MOUSE)
      handleMouse(w);
    else if(c == 'q') {
      break;
    }
    else
      handleKey(c, w);
  }
}

void handleMouse(world* w) {
  MEVENT e;
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
  default:
    return;
  }

  printGui("hrm", w);
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

