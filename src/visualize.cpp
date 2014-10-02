#include <particleFilter.h>

using namespace cv;
using namespace std;

void ParticleFilter::dispParticles()
{
  for(int i = 0; i< min(500, numParticles); i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    circle(frame, Point(particleY, particleX), 4, Scalar_<float>(0.,0.,1.), -1);
  }

  // circle(frame, Point(50, 100), 4, Scalar_<float>(0.,1.,0.), -1);

  return;
}

void ParticleFilter::dispAllParticles()
{
  for(int i = 0; i< numParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    circle(frame, Point(particleY, particleX), 4, Scalar_<float>(1.,0.,0.), -1);
  }

  // circle(frame, Point(50, 100), 4, Scalar_<float>(0.,1.,0.), -1);

  return;
}

void ParticleFilter::loadMapImage()
{
  for(uint i = 0; i < image.rows; i++) {
    for(uint j = 0; j < image.cols; j++) {
      float val = weanMap.prob[i][j];
      if (val >= 0) val = 1 - val;
      image.at<Point3f>(i,j) = Point3f(val, val, val);
    }
  }
}

void ParticleFilter::visualize()
{
  frame = image.clone();
  // dispParticles();
  dispAllParticles();

  namedWindow( "Wean Map", WINDOW_AUTOSIZE);  // Create a window for display.
  if (!frame.empty()) {
    imshow("Wean Map", frame);                  // Show our image inside it
  }

  waitKey(10);                                // Wait for a keystroke in the window
  return;
}
