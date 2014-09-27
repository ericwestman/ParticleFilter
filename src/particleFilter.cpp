#include <iostream>
#include <mapReader.h>
#include <logReader.h>

int main()
{
	// Load occupancy map of wean hall
  char weanMapName[] = "../data/map/wean.dat";
  MyMap *weanMap = new MyMap;
  read_map(weanMapName, weanMap);

  // Load data from the log

  std::cout << "Done!\n";
}