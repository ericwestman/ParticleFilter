#include <iostream>
#include <logReader.h>
#include <visualize.h>

using namespace std;

int main()
{
	// Setup for random number generator
	srand(time(NULL));

	// Load occupancy map of wean hall
  char weanMapName[] = "../data/map/wean.dat";
  MyMap *weanMap = new MyMap;
  vector<Particle> potentialParticles;
  read_map(weanMapName, weanMap, potentialParticles);

  // Load data from the log
  char logName[] = "../data/log/robotdata1.log";
  vector<float> timestamps;
  vector<LaserData> myLaserData;
  vector<OdometryData> myOdometryData;
  read_log(logName, myLaserData, myOdometryData, timestamps);

  // Draw initial particles
  vector<Particle> particles = drawParticles(potentialParticles);

  // Visualize building map
  for (int i = 0; i < timestamps.size(); i++) {
  	visualize(weanMap, particles);
  }

  std::cout << "Done!\n";
}