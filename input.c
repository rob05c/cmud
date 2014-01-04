#include "input.h"
#include <time.h>
#include <ncurses.h>
#include "gui.h"
#include "string.h"

/* 
   We want to put the tick in its own thread, but that isn't easy 
   That requires user input and tick to synchronize data access, e.g. via mutexes
*/
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
  if(n->Health <= 0)
    npc_kill(w, n);
}

void npc_kill(world* w, npc* n) {
  const int DEATH_TIME = 5;
  const int EXPERIENCE = 10;
  npc this = *n; /* yeah, I went there */

  w->Player.Experience += EXPERIENCE;
  this.Dead = 1;
  this.reviveTime = time(0) + DEATH_TIME; /* @todo fix this to not rely on time_t being seconds (it's not guaranteed) */
  npc_remove(&w->Npcs, n);
  npc_add_object(&w->deadNpcs, this);
}

void npc_revive(world* w, npc* n) {
  npc this = *n; /* copy, because n is stack-allocated and we're about to remove it */
  this.Dead = 0;
  this.Health = this.MaxHealth;
  npc_remove(&w->deadNpcs, n);
  npc_add_object(&w->Npcs, this);
  map_add_object(&w->Map, this.MapObject);
}
