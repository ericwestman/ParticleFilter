#include <cstdio>
#include <cstring>

struct MyMap{
  int size_x, size_y, min_x, min_y, max_x, max_y;
  int resolution, global_mapsize_x, global_mapsize_y, offset_x, offset_y;
  float prob[8000][8000];
};

int read_map(char *mapName, MyMap *occupancyMap);
