#pragma once
#include <cstdio>
#include <cstring>
#include <vector>
#include <particle.h>


struct MyMap{
  int size_x, size_y, min_x, min_y, max_x, max_y;
  int resolution, global_mapsize_x, global_mapsize_y, offset_x, offset_y;
  float prob[800][800];
};

int read_map(char *mapName, MyMap *occupancyMap, std::vector<Particle>& potentialParticles);
