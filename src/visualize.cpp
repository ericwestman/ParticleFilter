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

  return;
}


Scalar_<float> getColor(int i)
{
// fills in a gradient from the color <sr,sg,sb> to <er,eg,eb>

	int steps = 100;
	float n;
	unsigned char r, g, b;

	float sr = 0;
	float sg = 0;
	float sb = 1;

	float er = 1;
	float eg = 0;
	float eb = 0;

	for( i = 0; i < steps; i++ )
	{
		n = (float)i / (float) (steps-1);
		r = sr * (1.0f-n) + er * n;
		g = sg * (1.0f-n) + eg * n;
		b = sb * (1.0f-n) + eb * n;

	}
	return Scalar_<float>(r,g,b);
}


int getMaxWeightParticle(vector<float> weights)
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



void ParticleFilter::dispAllParticles()
{

  int maxIdx = getMaxWeightParticle(weights);

  for(int i = 0; i< numParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    if (i != maxIdx)
    	circle(frame, Point(particleY, particleX), 1, Scalar_<float>(1.,0.,0.), -1);
    else
    	circle(frame, Point(particleY, particleX), 4, Scalar_<float>(0.,0.,1.), -1);


  }

  return;
}

void ParticleFilter::dispTestParticles()
{
  for(int i = 0; i< numTestParticles; i++) {
    int particleX = particles[i].getX();
    int particleY = particles[i].getY();

    if (i < numTestParticles/2)
      circle(frame, Point(particleY, particleX), 1, Scalar_<float>(1.,0.,0.), -1);
    else
      circle(frame, Point(particleY, particleX), 1, Scalar_<float>(0.,0.,1.), -1);

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

