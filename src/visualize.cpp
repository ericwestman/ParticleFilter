#include <particleFilter.h>

using namespace cv;
using namespace std;

int getMaxWeightParticle(vector<float>& weights)
{
  // get the index of the highest weighted particle
  float weightMax = 0;
    int maxIdx = 0;
    for (int i = 0; i < weights.size(); i++) 
    {
      if (weights[i] > weightMax)
      {
        weightMax = weights[i];
        maxIdx = i;
      }
    }
    return maxIdx;
}


void ParticleFilter::dispParticles()
{
  int maxIdx = getMaxWeightParticle(weights);

  for(int i = 0; i< numParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();


    float color = weights[i]/((double) weights[maxIdx]);

    frame.at<Point3f>(800 - particleY, particleX) = Point3f(1-color, 0, color);
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

void ParticleFilter::writeVideo()
{
  Mat img;
  frame.convertTo(img, CV_8UC3, 255);

  if (!outputVideo.isOpened())
  {
      cout  << "Could not open the output video for write" << endl;
      return;
  }
  
  outputVideo << img;

  cout << "Finished writing frame" << endl;
  return;
}

void ParticleFilter::visualize()
{
  frame = image.clone();
  dispParticles();

  namedWindow( "Wean Map", WINDOW_AUTOSIZE);  // Create a window for display.
  if (!frame.empty()) {
    imshow("Wean Map", frame);                  // Show our image inside it
  }

  if (saveVideo) writeVideo();

  waitKey(10);                                // Wait for a keystroke in the window
  return;
}

