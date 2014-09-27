#include <iostream>
#include <mapReader.h>
#include <logReader.h>

using namespace std;

int main()
{
	// Load occupancy map of wean hall
  char weanMapName[] = "../data/map/wean.dat";
  MyMap *weanMap = new MyMap;
  read_map(weanMapName, weanMap);

  // Load data from the log
  char logName[] = "../data/log/robotdata1.log";
  vector<LaserData> *myLaserData = new vector<LaserData>;
  vector<OdometryData> *myOdometryData = new vector<OdometryData>;
  read_log(logName, myLaserData, myOdometryData);

  std::cout << "Done!\n";
}