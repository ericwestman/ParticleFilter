#include <iostream>
#include <mapReader.h>
#include <logReader.h>

int main()
{
  char weanMapName[] = "../data/map/wean.dat";
  MyMap *weanMap = new MyMap;
  read_map(weanMapName, weanMap);
  std::cout << "Hello World!\n";
}