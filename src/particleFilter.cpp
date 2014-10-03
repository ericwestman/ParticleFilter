#include <iostream>
#include <particleFilter.h>

using namespace std;
using namespace cv;

ParticleFilter::ParticleFilter() 
{
  // Initialize class members
  char name1[] = "../data/map/wean.dat";
  weanMapName = name1;
  weanMap = MyMap();
  image = Mat(800,800,CV_32FC3);
  frame = Mat(800,800,CV_32FC3);

  logName = "../data/log/robotdata1.log";

  numParticles = 5000;
  numTestParticles = 100;


  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  xy_normal = std::normal_distribution<double>(0.0,2.0);
  theta_normal = std::normal_distribution<double>(0.0,2.0*M_PI/180);
}

int main()
{
	// Setup for random number generator
	srand(time(NULL));

  // Initialize our particle filter
  ParticleFilter filter;

	// Load occupancy map of wean hall and data from log
  filter.readMap();
  filter.loadMapImage();
  filter.readLog();

  // Draw initial particles
  filter.drawParticles();
  filter.visualize();

  // Start the filter!
  for (int i = 1; i < filter.timestamps.size(); i++) {
    filter.motionModel(i);
    filter.visualize();
    // filter.updateWeights_test();
    filter.updateWeightsCV(i);
    filter.resampleParticles();
  }

  std::cout << "Done!\n";
}

// // Test Particles
// int main()
// {
//   // Setup for random number generator
//   srand(time(NULL));

//   // Initialize our particle filter
//   ParticleFilter filter;

//   // Load occupancy map of wean hall and data from log
//   filter.readMap();
//   filter.loadMapImage();
//   filter.readLog();

//   // Draw initial particles
//   cout << "Drawing test particles " << endl;
//   filter.drawTestParticles();
//   filter.visualizeTestParticles();

//   // Start the filter!
//   for (int i = 1; i < filter.timestamps.size(); i++) {
//     filter.motionModel(i);
//     filter.visualizeTestParticles();
//     // filter.updateWeights_test();
//     filter.updateWeightsCV(i);
//     filter.resampleParticles();
//   }

//   std::cout << "Done!\n";
// }

