#include <particleFilter.h>

#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

int ParticleFilter::readMap()
{
  int x, y, count;
  float temp;
  char line[256];
  FILE *fp;


  if((fp = fopen(weanMapName, "rt")) == NULL) {
    fprintf(stderr, "# Could not open file %s\n", weanMapName);
    return -1;
  }
  while((fgets(line, 256, fp) != NULL)
	&& (strncmp("global_map[0]", line , 13) != 0)) {
    if(strncmp(line, "robot_specifications->autoshifted_x", 35) == 0)
      if(sscanf(&line[35], "%d", &(weanMap.offset_x)) != 0) {
	weanMap.offset_x = weanMap.offset_x;
      }
    if(strncmp(line, "robot_specifications->autoshifted_y", 35) == 0) {
      if (sscanf(&line[35], "%d", &(weanMap.offset_y)) != 0) {
	weanMap.offset_y = weanMap.offset_y;
      }
    }
  }

  if(sscanf(line,"global_map[0]: %d %d", &weanMap.size_y, &weanMap.size_x) != 2) {
    fprintf(stderr, "ERROR: corrupted file %s\n", weanMapName);
    fclose(fp);
    return -1;
  }

  weanMap.min_x = weanMap.size_x;
  weanMap.max_x = 0;
  weanMap.min_y = weanMap.size_y;
  weanMap.max_y = 0;
  count = 0;
  for(x = 0; x < weanMap.size_x; x++)
    for(y = 0; y < weanMap.size_y; y++, count++) {
      
      fscanf(fp,"%e", &temp);
      if(temp < 0.0)
        weanMap.prob[x][y] = -1;
      else {
      	if(x < weanMap.min_x)
      	  weanMap.min_x = x;
      	else if(x > weanMap.max_x)
      	  weanMap.max_x = x;
      	if(y < weanMap.min_y)
      	  weanMap.min_y = y;
      	else if(y > weanMap.max_y)
      	  weanMap.max_y = y;
      	
        weanMap.prob[x][y] = 1 - temp;	   
        if (temp > 0.8) {
          Particle location(x, y, 0.0);
          potentialParticles.push_back(location);
        }
      }
    }
  fclose(fp);
  return 0;
}
