#include <iostream>
#include <logReader.h>
#include <visualize.h>

using namespace std;

int main()
{
	// Load occupancy map of wean hall
  char weanMapName[] = "../data/map/wean.dat";
  MyMap *weanMap = new MyMap;
  vector<Particle> potentialParticles;
  read_map(weanMapName, weanMap, potentialParticles);

  // Load data from the log
  char logName[] = "../data/log/robotdata1.log";
  vector<LaserData> *myLaserData = new vector<LaserData>;
  vector<OdometryData> *myOdometryData = new vector<OdometryData>;
  read_log(logName, myLaserData, myOdometryData);

  // Visualize building map
  visualize(weanMap, potentialParticles);

  std::cout << "Done!\n";
}