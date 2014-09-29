#include <iostream>
#include <ParticleFilter.h>

using namespace std;

ParticleFilter::ParticleFilter() 
{
  weanMapName = "../data/map/wean.dat";
  weanMap = new MyMap;
  // std::vector<Particle> potentialParticles;
  // std::vector<Particle> particles;

  logName = "../data/log/robotdata1.log";
  // std::vector<float> timestamps;
  // std::vector<LaserData> logLaserData;
  // std::vector<OdometryData> logOdometryData;
}

int main()
{
	// Setup for random number generator
	srand(time(NULL));

  // Initialize our particle filter
  ParticleFilter filter;

	// Load occupancy map of wean hall
  filter.readMap();

  // Load data from the log
  filter.readLog();

  // Draw initial particles
  filter.drawParticles();

  // Visualize building map
  for (int i = 0; i < filter.timestamps.size(); i++) {
  	filter.visualize();
  }

  std::cout << "Done!\n";
}