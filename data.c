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
  point p = {x, y};
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
  map_object object = {next_id(), point_create(x, y), symbol, color};
  return object;
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
  if(initialCapacity < 1)
    initialCapacity = 1;
  map_list list;
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
  map_object* newObjects = malloc(sizeof(map_object) * (long unsigned int)(*list.Size) * 2);
  memcpy(newObjects, list.Objects, sizeof(map_object) * (long unsigned int)(*list.Length));
  *list.Size *= 2;
  map_object* oldObjects = list.Objects;
  list.Objects = newObjects;
  free(oldObjects);
  return list;
}

map_list map_add(map_list list, int x, int y, char symbol, short color) {
  map_object object = {next_id(), point_create(x, y), symbol, color};
  return map_add_object(list, object);
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
    if(map_object_equals(object, iobject)) {
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

npc npc_create(int x, int y, char symbol, short color, const char* name, const char* desc, short max_health) {
  npc n; /* = {map_object_create(x, y, symbol, color), name, desc, max_health, max_health}; */
  n.MapObject = map_object_create(x, y, symbol, color);
  n.Name = name;
  n.Desc = desc;
  n.MaxHealth = max_health;
  n.Health = max_health;
  return n;
}

void world_refresh(world* w)
{
  windows_refresh(w->Windows);
}
