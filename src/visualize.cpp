#include <particleFilter.h>

using namespace cv;
using namespace std;

void ParticleFilter::dispParticles()
{
  for(int i = 0; i< min(500, numParticles); i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    circle(frame, Point(particleY, 800 - particleX), 4, Scalar_<float>(0.,0.,1.), -1);
  }

  return;
}

void ParticleFilter::dispAllParticles()
{
  for(int i = 0; i< numParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    circle(frame, Point(particleX, 800 - particleY), 4, Scalar_<float>(1.,0.,0.), -1);
  }

  return;
}

void ParticleFilter::dispTestParticles()
{
  for(int i = 0; i< numTestParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    if (i < numTestParticles/2)
      circle(frame, Point(particleY, 800 - particleX), 4, Scalar_<float>(1.,0.,0.), -1);
    else
      circle(frame, Point(particleY, 800 - particleX), 4, Scalar_<float>(0.,0.,1.), -1);

    if (particles[i].getX() < 280 && particles[i].getX() > 270 && particles[i].getY() < 430 && particles[i].getY() > 420)
      cout << "Good particle weight: " << weights[i] << endl;
    else 
      cout << "Bad particle weight: " << weights[i] << endl;

  }

  return;
}

void ParticleFilter::loadMapImage()
{
  for(uint i = 0; i < image.rows; i++) {
    for(uint j = 0; j < image.cols; j++) {
      float val = weanMap.prob[i][800-j];
      if (val >= 0) val = 1 - val;
      image.at<Point3f>(j,i) = Point3f(val, val, val);
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


void ParticleFilter::visualizeTestParticles()
{
  frame = image.clone();
  // dispParticles();
  dispTestParticles();

  namedWindow( "Wean Map", WINDOW_AUTOSIZE);  // Create a window for display.
  if (!frame.empty()) {
    imshow("Wean Map", frame);                  // Show our image inside it
  }

  waitKey(0);                                // Wait for a keystroke in the window
  return;
}

