#ifndef datah
#define datah

#include <stddef.h> /* needed for size_t */
#include <ncurses.h> /* needed for WINDOW */

typedef long long id_t;
id_t next_id(); /* totally not threadsafe  */

/* this ought to be in gui.h, but that would be a circular dependency */
typedef struct {
  WINDOW* Main;
  WINDOW* Status;
} windows;

typedef struct {
  int X;
  int Y;
} point;
point point_create(int x, int y);
int point_equals(point* a, point* b); /* @returns 0 iff points are equal */

typedef struct {
  id_t Id;
  point Point;
  char Symbol; /* @todo make this work with multibyte unicode characters */
  short Color;
} map_object;
map_object map_object_create(int x, int y, char symbol, short color);
int map_object_equals(map_object* a, map_object* b); /* @returns 0 iff objects are equal */


typedef struct {
  map_object* Objects;
  size_t* Length;
  size_t* Size;
} map_list;
map_list map_create(size_t initialCapacity);
map_list map_add(map_list map, int x, int y, char symbol, short color);
map_list map_add_object(map_list map, map_object object);
map_list map_remove_object(map_list map, map_object* object); /* removes the first object in the list which matches the given object */
map_list map_objects_in_box(map_list list, int top, int left, int bottom, int right); /* @returns a new map_list of the objects within the given box. New map_list is new, not a modification of the old list */

typedef struct {
  map_object MapObject;
  const char* Name;
  const char* Desc;
  short MaxHealth;
  short Health;
} npc;
npc npc_create(int x, int y, char symbol, short color, const char* name, const char* desc, short max_health);

typedef struct {
  map_list Map;
  point Location;
  short Color;
  const char* Symbol;
  windows Windows;
  int WindowWidth;
  int WindowHeight;
} world;

void world_refresh(world* w);

#endif
