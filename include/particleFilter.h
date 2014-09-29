#pragma once

#include <types.h>
#include <vector>

class ParticleFilter {
public:
  ParticleFilter();
	int readMap();
	void readLog();
	void drawParticles();
  void motionModel();
  void visualize();

  std::vector<float> timestamps;

private:
  char *weanMapName;
  MyMap *weanMap;
  std::vector<Particle> potentialParticles;
  std::vector<Particle> particles;

  char *logName;
  std::vector<LaserData> logLaserData;
  std::vector<OdometryData> logOdometryData;
};