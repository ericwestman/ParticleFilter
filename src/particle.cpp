#include <particleFilter.h>

#include <vector>
#include <numeric>
#include <math.h>
#include <stdlib.h>

#include <iterator>
#include <ctime>
#include <type_traits>
#include <cassert>

#define TWO_PI 6.2831853071795864769252866

using namespace std;

void ParticleFilter::drawParticles()
{
  // Set up random number generator for heading
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_real_distribution<float> heading(-M_PI, M_PI);

  for(int i = 0; i < numParticles; ++i){
    // Get random particle from map
    int j = rand() % potentialParticles.size();

    // Assign random heading to particle
    Particle p = potentialParticles[j];


    //Particle p(400.0,450.0,0.0);
    //Particle p(100.0,150.0,0.0);
    p.setTheta(heading(mt));
    // cout << "ParticleTheta: " << p.getTheta() << " and random: " << heading(mt) << endl;

    particles.push_back(p);
  }
  return;
}

void ParticleFilter::motionModel(int &timestep)
{
	for (int i = 0; i < particles.size(); ++i) {
		float xrand = xy_normal(generator);
		float yrand = xy_normal(generator);
    float trand = theta_normal(generator);

    // We're only using the laser measurements (with odometry data), as opposed to all
    // of the odometry measurements that we get in between (maybe change later)
		float x = particles[i].getX() + (logLaserData[timestep].x - logLaserData[timestep - 1].x)/10.0
												 + xrand; // random displacement in grid cell units
		float y = particles[i].getY() + (logLaserData[timestep].y - logLaserData[timestep - 1].y)/10.0
                         + yrand; // random displacement in grid cell units
    float t = particles[i].getTheta() + logLaserData[timestep].theta - logLaserData[timestep - 1].theta 
                         + trand; // random displacement in radians

		particles[i] = Particle(x, y, t);

	}
  return;
}


float euclidDist(Coord start, Coord end) 
{
  return sqrt(pow((start.row - end.row),2) + pow((start.col - end.col),2));
}


float observationModel(float x, float mu)
{
  // Gaussian component
  float sigma_g = 5;
  float g_max = 1.0;
  float gaussian = g_max*exp(-((mu - x) * (mu - x)) / ((sigma_g * sigma_g) * 2.0));

  // Max range component
  float max_range_prob = g_max/4.0;
  float max_range = (x > 818) ? max_range_prob : 0;

  // Exponential component
  float sigma_e = -0.005;
  float a = 4;
  float exponential = g_max/a*exp(sigma_e*x);

  // Uniform component
  float uniform = g_max / 10;

  // cout << gaussian + exponential + uniform + max_range << endl;

  return gaussian + exponential + uniform + max_range;
}

float ParticleFilter::calculateWeightCV(Particle &p, int &timestep) {
  float wallProb = 0.8;
  float particleWeight = 0;

  frame = image.clone();

  Coord startCell;
  startCell.row = min(int(round(p.getX())),800);
  startCell.col = min(int(round(p.getY())),800);

  for (int a = 0; a < 180; a++) {

    Coord endCell;
    endCell.row = int(round(p.getX()+800*cos(p.getTheta()+(a-90)*M_PI/180)));
    endCell.col = int(round(p.getY()+800*sin(p.getTheta()+(a-90)*M_PI/180)));

    // grabs pixels along the line (pt1, pt2)
    // from 8-bit 3-channel image to the buffer
    cv::LineIterator it(image, cv::Point(startCell.col, startCell.row), 
                               cv::Point(endCell.col, endCell.row), 8);

    // alternative way of iterating through the line
    for(int i = 0; i < it.count; i++, ++it) {

      // Display the ray
      if (a < 110 && a > 70) frame.at<cv::Point3f>(it.pos()) = cv::Point3f(1., 0., 0.);
      else frame.at<cv::Point3f>(it.pos()) = cv::Point3f(0., 1., 0.);

      // If the ray goes outside of the image before finding a wall, give low weight
      if (it.pos().x >= 800 || it.pos().y >= 800) { 
        particleWeight += log(0.1);
        break;
      }

      float val = image.at<cv::Point3f>(it.pos()).x;

      // If the ray hits a wall, then use the observation model
      if (1 - val >= wallProb && val >= 0)
      {
        int rowDist = it.pos().y - startCell.row;
        int colDist = it.pos().x - startCell.col;
        
        float particleDistToWall = sqrt(rowDist*rowDist + colDist*colDist);
        float laserDistToWall = logLaserData[timestep].r[a];
        float p = observationModel(laserDistToWall, particleDistToWall)*5.0;
   

        //cout << p << endl;
        particleWeight += log(p);
        
        //particleWeight += log(p/(1-p));
        break;
      }
      // If the ray sees something outside the map that we don't know about, 
      else if (val == -1) {
        particleWeight += log(0.3);
        break;
      }
    }

    // // Optionally display every ray and the end point that it compares with the laser data
    // cv::circle(frame, it.pos(), 4, cv::Scalar_<float>(0.,0.,1.), -1);
    // cv::namedWindow( "Wean Map", cv::WINDOW_AUTOSIZE);
    // if (!frame.empty()) {
    //   cv::imshow("Wean Map", frame);
    // }
    // cout << "Weight: " << particleWeight << " StartRow: " << startCell.row << " StartCol: " << startCell.col << " ItPosY: " << it.pos().y << " ItPosX: " << it.pos().x << endl;
    // cv::waitKey(1000);

  }

  // cout << exp(particleWeight) << endl;

  return exp(particleWeight);
  //return 1.0 - 1.0/(1.0 + exp(particleWeight));
}

void ParticleFilter::updateWeights_test()
{
  weights.clear();
  intervals.clear();

  weights.push_back(1000.);
  intervals.push_back(0);

  for (int i = 1; i < particles.size(); ++i) {
    weights.push_back(0.1);
    intervals.push_back(i);
  }
  intervals.push_back(particles.size());

  return;
}

void ParticleFilter::updateWeightsCV(int &timestep)
{
  weights.clear();
  intervals.clear();

  for (int p = 0; p <= particles.size(); p++) {
    weights.push_back(calculateWeightCV(particles[p],timestep));
    intervals.push_back(p);
  }

  return;
}



void ParticleFilter::resampleParticles()
{
  // Set up distribution
  std::piecewise_constant_distribution<> dist(begin(intervals),
                                              end(intervals),
                                              begin(weights));

  // Copy the current particles and clear the vector
  vector<Particle> oldParticles = particles;
  particles.clear();

  for (int i = 0; i < numParticles; ++i)
  {
      // Generate random number using gen, distributed according to dist
      int r = (int) dist(generator);
      // Push the new particle into the vector
      particles.push_back(oldParticles[r]);
  }

  return;
}

