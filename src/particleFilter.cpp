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


  // File names and video writer
  logName   = "../data/log/robotdata4.log";
  videoName = "../data/videos/robotdata4.mpg";
  saveVideo = false;
  outputVideo.open(videoName, CV_FOURCC('M', 'P', 'E', 'G'), 30, image.size(), true);

  // Particles
  numParticles = 1000;

  // Distribution
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  xy_normal = std::normal_distribution<double>(0.0,1.0);
  theta_normal = std::normal_distribution<double>(0.0,1.0*M_PI/180);
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

  // Start the filter!
  for (int i = 1; i < filter.timestamps.size(); i++) {
    filter.motionModel(i);
    filter.updateWeights(i);
    filter.resampleParticles();

    //filter.estimatePosition_maxWeight();
    filter.estimatePosition_weightedAverage();
    //filter.estimatePosition_average();

    filter.visualizeWithRays(i);
    //filter.visualize();
  }

  std::cout << "Done!\n";
}


