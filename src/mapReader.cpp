#include <mapReader.h>

using namespace std;

int read_map(char *mapName, MyMap *occupancyMap, vector<Particle>& potentialParticles)
{
  int x, y, count;
  float temp;
  char line[256];
  FILE *fp;


  if((fp = fopen(mapName, "rt")) == NULL) {
    fprintf(stderr, "# Could not open file %s\n", mapName);
    return -1;
  }
  fprintf(stderr, "# Reading map: %s\n", mapName);
  while((fgets(line, 256, fp) != NULL)
	&& (strncmp("global_map[0]", line , 13) != 0)) {
    if(strncmp(line, "robot_specifications->resolution", 32) == 0)
      if(sscanf(&line[32], "%d", &(occupancyMap->resolution)) != 0)
	printf("# Map resolution: %d cm\n", occupancyMap->resolution);
    if(strncmp(line, "robot_specifications->autoshifted_x", 35) == 0)
      if(sscanf(&line[35], "%d", &(occupancyMap->offset_x)) != 0) {
	occupancyMap->offset_x = occupancyMap->offset_x;
	printf("# occupancyMap offsetX: %d cm\n", occupancyMap->offset_x);
      }
    if(strncmp(line, "robot_specifications->autoshifted_y", 35) == 0) {
      if (sscanf(&line[35], "%d", &(occupancyMap->offset_y)) != 0) {
	occupancyMap->offset_y = occupancyMap->offset_y;
	printf("# occupancyMap offsetY: %d cm\n", occupancyMap->offset_y);
      }
    }
  }

  if(sscanf(line,"global_map[0]: %d %d", &occupancyMap->size_y, &occupancyMap->size_x) != 2) {
    fprintf(stderr, "ERROR: corrupted file %s\n", mapName);
    fclose(fp);
    return -1;
  }
  printf("# occupancyMap size: %d %d\n", occupancyMap->size_x, occupancyMap->size_y);

  occupancyMap->min_x = occupancyMap->size_x;
  occupancyMap->max_x = 0;
  occupancyMap->min_y = occupancyMap->size_y;
  occupancyMap->max_y = 0;
  count = 0;
  for(x = 0; x < occupancyMap->size_x; x++)
    for(y = 0; y < occupancyMap->size_y; y++, count++) {
      if(count % 10000 == 0)
	fprintf(stderr, "\r# Reading ... (%.2f%%)",
		count / (float)(occupancyMap->size_x * occupancyMap->size_y) * 100);
      
      fscanf(fp,"%e", &temp);
      if(temp < 0.0)
        occupancyMap->prob[x][y] = -1;
      else {
      	if(x < occupancyMap->min_x)
      	  occupancyMap->min_x = x;
      	else if(x > occupancyMap->max_x)
      	  occupancyMap->max_x = x;
      	if(y < occupancyMap->min_y)
      	  occupancyMap->min_y = y;
      	else if(y > occupancyMap->max_y)
      	  occupancyMap->max_y = y;
      	
        occupancyMap->prob[x][y] = 1 - temp;	   
        if (temp > 0.8) {
          Particle location;
          location.x = x;
          location.y = y;
          location.theta = 0;
          potentialParticles.push_back(location);
        }
      }
    }
  fprintf(stderr, "\r# Reading ... (%.2f%%)\n\n",
	  count / (float)(occupancyMap->size_x * occupancyMap->size_y) * 100);
  fclose(fp);
  return 0;
}
