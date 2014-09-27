#include <visualize.h>

using namespace cv;
using namespace std;

void visualize(MyMap *occupancyMap, vector<Particle>& potentialParticles)
{
    Mat image(800,800,CV_32FC3);
    for(uint i = 0; i < image.rows; i++)
    {
      for(uint j = 0; j < image.cols; j++)
      {
        float val = (*occupancyMap).prob[i][j];
        if (val >= 0) val = 1 - val;
        image.at<Point3f>(i,j) = Point3f(val, val, val);
      }
    }

    vector<Particle> v;
    for(int i = 0; i < 100; i++){
      int j = rand() % potentialParticles.size();
      v.push_back(potentialParticles[j]);
    }

    dispParticles(image, v);

    namedWindow( "Wean Map", WINDOW_AUTOSIZE );   // Create a window for display.
    imshow( "Wean Map", image );                  // Show our image inside it.

    waitKey(0);                                   // Wait for a keystroke in the window
    return;
}

void dispParticles(Mat &image, vector<Particle> &samples)
{
  for(int i = 0; i< samples.size(); i++){
    int particleX = samples[i].x;
    int particleY = samples[i].y;

    circle(image, Point(particleY, particleX), 4, Scalar_<float>(0.,0.,1.), -1);
  }

  return;
}