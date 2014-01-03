#include "data.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "gui.h"

id_t next_id() {
  static id_t id;
  return id++;
}

point point_create(int x, int y) {
  point p;
  p.X = x;
  p.Y = y;
  return p;
}

int point_equals(point* a, point* b) {
  if(a == NULL || b == NULL)
    return -1;
  if(a->X != b->X)
    return -1;
  if(a->Y != b->Y)
    return -1;
  return 0;
}

map_object map_object_create(int x, int y, char symbol, short color) {
  map_object o;
  o.Id = next_id();
  o.Point.X = x;
  o.Point.Y = y;
  o.Symbol = symbol;
  o.Color = color;
  return o;
}

int map_object_equals(map_object* a, map_object* b) {
  if(a == NULL || b == NULL)
    return -1;
  return a->Id == b->Id ? 0 : -1;
  /*
  if(point_equals(&a->Point, &b->Point) != 0)
    return -1;
  if(a->Symbol != b->Symbol)
    return -1;
  if(a->Color != b->Color)
    return -1;
  return 0;
  */
}


map_list map_create(size_t initialCapacity) {
  map_list list;

  if(initialCapacity < 1)
    initialCapacity = 1;
  list.Objects = malloc(sizeof(map_object) * (long unsigned int)initialCapacity);
  list.Length = malloc(sizeof(size_t));
  list.Size = malloc(sizeof(size_t));
  *list.Length = 0;
  *list.Size = initialCapacity;
  return list;
}

void map_destroy(map_list list) {
  free(list.Objects);
  free(list.Length);
  free(list.Size);
}

map_list map_reallocate(map_list list) {
  const int FACTOR = 2;
  *list.Size *= FACTOR;
  list.Objects = realloc(list.Objects, (sizeof(map_object) * (long unsigned int)(*list.Size) * FACTOR));
  return list;
}

map_list map_add(map_list list, int x, int y, char symbol, short color) {
  map_object o;
  o.Id = next_id();
  o.Point.X = x;
  o.Point.Y = y;
  o.Symbol = symbol;
  o.Color = color;
  return map_add_object(list, o);
}

map_list map_add_object(map_list list, map_object object) {
  if(*list.Length == *list.Size) {
    list = map_reallocate(list);
  }
  list.Objects[*list.Length] = object;
  ++(*list.Length);
  return list;
}

map_list map_remove_object(map_list list, map_object* object) {
  int i;
  int end;
  map_object* iobject;
  for(i = 0, end = *list.Length; i != end; ++i) {
    iobject = &list.Objects[i];
    if(map_object_equals(object, iobject) == 0) {
      break;
    }
  }
  for(end = end - 1; i != end; ++i) {
    list.Objects[i] = list.Objects[i+1];
  }
  --(*list.Length);
  return list;
}

map_list map_objects_in_box(map_list list, int top, int left, int bottom, int right) {
  map_list objects = map_create(0);
  map_object* o;
  int i;
  int end;
  for(i = 0, end = *list.Length; i != end; ++i) {
    o = &list.Objects[i];
    if(o->Point.X >= left && o->Point.X <= right && o->Point.Y >= top && o->Point.Y <= bottom)
      objects = map_add_object(objects, *o);
  }
  return objects;
}

map_object* map_get(map_list list, point p) {
  map_object o;
  int i;
  int end;
  for(i = 0, end = *list.Length; i != end; ++i) {
    o = list.Objects[i];
    if(point_equals(&o.Point, &p) == 0) {
      return &list.Objects[i];
    }
  }
  return NULL;
}


npc_list npc_list_create(size_t initialCapacity, map_list mapList) {
  npc_list list;

  if(initialCapacity < 1)
    initialCapacity = 1;
  list.Npcs = malloc(sizeof(npc) * (long unsigned int)initialCapacity);
  list.Length = malloc(sizeof(size_t));
  list.Size = malloc(sizeof(size_t));
  *list.Length = 0;
  *list.Size = initialCapacity;
  list.MapList = mapList;
  return list;
}

void npc_list_destroy(npc_list list) {
  free(list.Npcs);
  free(list.Length);
  free(list.Size);
}


int npc_equals(npc* a, npc* b) {
  return a->MapObject.Id == b->MapObject.Id ? 0 : 1;
}

npc npc_create(int x, int y, char symbol, short color, const char* name, const char* desc, short max_health) {
  npc n; /* = {map_object_create(x, y, symbol, color), name, desc, max_health, max_health}; */
  n.MapObject = map_object_create(x, y, symbol, color);
  n.Name = name;
  n.Desc = desc;
  n.MaxHealth = max_health;
  n.Health = max_health;
  return n;
}

npc_list npc_reallocate(npc_list list) {
  const int FACTOR = 2;
  *list.Size *= FACTOR;
  list.Npcs = realloc(list.Npcs, (sizeof(map_object) * (long unsigned int)(*list.Size) * FACTOR));
  return list;
}

npc_list npc_add(npc_list list, 
		 int x, 
		 int y, 
		 char symbol, 
		 short color, 
		 const char* name, 
		 const char* desc, 
		 short maxHealth,
		 int aggro) 
{
  map_object o;
  npc n;

  o.Id = next_id();
  o.Point.X = x;
  o.Point.Y = y;
  o.Symbol = symbol;
  o.Color = color;
  list.MapList = map_add_object(list.MapList, o);

  n.MapObject = o;
  n.Name = name;
  n.Desc = desc;
  n.MaxHealth = maxHealth;
  n.Health = maxHealth;
  n.Aggro = aggro;

  return npc_add_object(list, n);
}

npc_list npc_add_object(npc_list list, npc n) {
  if(*list.Length == *list.Size) {
    list = npc_reallocate(list);
  }
  list.Npcs[*list.Length] = n;
  ++(*list.Length);
  return list;
}

npc_list npc_remove(npc_list list, npc* n) {
  int i;
  int end;
  npc* inpc;
  for(i = 0, end = *list.Length; i != end; ++i) {
    inpc = &list .Npcs[i];
    if(npc_equals(n, inpc) == 0) {
      break;
    }
  }
  for(end = end - 1; i != end; ++i) {
    list.Npcs[i] = list.Npcs[i+1];
  }
  --(*list.Length);
  return list;
}

npc* npc_get(npc_list list, id_t id) {
  npc n;
  int i;
  int end;
  for(i = 0, end = *list.Length; i != end; ++i) {
    n = list.Npcs[i];
    if(n.MapObject.Id == id) {
      return &list.Npcs[i];
    }
  }
  return NULL;
}

void world_refresh(world* w)
{
  windows_refresh(w->Windows);
}

/* 
   @todo put the tick in a thread.
         Until it is, slow ticks will make player input hang.
*/
void tick(world* w) {
  npc* n;
  int i;
  int end;
  npc_list list = w->Npcs;

  printMessage("", w);
  for(i = 0, end = *list.Length; i != end; ++i) {
    n = &list.Npcs[i];
    npc_tick(w, n);
  }
}

void npc_tick(world* w, npc* n) {
  if(point_equals(&n->MapObject.Point, &w->Player.Location) == 0) {
    if(n->Aggro) {
      char msg[200];
      strcpy(msg, "A ");
      strcat(msg, n->Name);
      strcat(msg, " attacks you savagely.");
      printMessage(msg, w);
      if(w->Player.Health >= 10)
	w->Player.Health -= 10;
      else
	w->Player.Health = 0;
    }
  }
  printGui(w);
  windows_refresh(w->Windows);
}
