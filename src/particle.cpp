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
    Particle p = potentialParticles[j];

    //Particle p = Particle(400.0,410.0,-M_PI/2);

    // Assign random heading to particle
    p.setTheta(heading(mt));
    particles.push_back(p);
  }
  return;
}

void ParticleFilter::drawTestParticles()
{
  // Set up random number generator for heading
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_real_distribution<float> heading(-M_PI, M_PI);

  Particle p = Particle(0.,0.,0.);

  for(int i = 0; i < numTestParticles; ++i){

    // Test stuff
    // if (i == 0) {
    //   // Get random particle around the neighborhood of the real robot
    //   p = Particle(400.0, 410.0, -M_PI/2);//-M_PI/2);
    //   // p = Particle(275.0 + rand() % 5, 425.0 + rand() % 5, heading(mt));
    // }
    // else if (i == 1){
    //   // int j = rand() % potentialParticles.size();
    //   // p = potentialParticles[j];
    //   // p.setTheta(heading(mt));

    //   p = Particle(650.0, 465.0, -M_PI/2);//M_PI/2);

    // }
    // else {
    //   int j = rand() % potentialParticles.size();
    //   p = potentialParticles[j];
    //   p.setTheta(heading(mt));
    // }

    p = Particle(400.0+ rand() % 5, 410.0+ rand() % 5, -M_PI/2 + rand() % 1); //-M_PI/2);
    //   // p = Particle(275.0 + rand() % 5, 425.0 + rand() % 5, heading(mt));

    // cout << "X " << p.getX() << " Y " << p.getY() << " T " << p.getTheta() << endl;
    particles.push_back(p);
  }

  return;
}

void ParticleFilter::motionModel(int timestep)
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

    // Check bounds
    if (x > 800.0)  x = 800.0;
    if (x < 0.0)    x = 0.0;
    if (y > 800.0)  y = 800.0;
    if (y < 0.0)    y = 0.0;

		particles[i] = Particle(x, y, t);

	}
  return;
}


float observationModel(float x, float mu)
{
  // Gaussian component
  float sigma_g = 15;
  float g_max = 1.0;
  float gaussian = g_max*exp(-((mu - x) * (mu - x)) / ((sigma_g * sigma_g) * 2.0));

  // Max range component
  float max_range_prob = g_max/3.0;
  float max_range = (x > 818.0) ? max_range_prob : 0.0;

  // Uniform component
  float uniform = g_max * 1.0/4.0;

  // Exponential component
  float sigma_e = -0.0008;
  float a = g_max*1.0/3.0;
  float exponential = a*exp(sigma_e*x);

  // return max( max ( max(gaussian, uniform), max_range), exponential);
  return gaussian + exponential + uniform + max_range;
  // return gaussian + uniform + max_range;
}

float ParticleFilter::calculateWeightCV(Particle &p, int timestep) {
  float wallProb = 0.55;
  float particleWeight = 0.0;

  // For drawing the individual rays / particles
  // frame = image.clone();

  ImageCoord startCell = ImageCoord(p.getLoc());

  for (int a = 0; a < 180; a+=4) {

    ImageCoord endCell = ImageCoord();
    endCell.row = 800 - int(round(p.getX()+200*cos(p.getTheta()+(a-90)*M_PI/180)));
    endCell.col = int(round(p.getY()+200*sin(p.getTheta()+(a-90)*M_PI/180)));

    // grabs pixels along the line (pt1, pt2)
    // from 8-bit 3-channel image to the buffer
    cv::LineIterator it(image, cv::Point(startCell.col, startCell.row), 
                               cv::Point(endCell.col, endCell.row), 8);


    // // Display the ray of the laser in the corner
    // int start = 150;
    // int laserDist = logLaserData[timestep].r[a]/10.0;
    // cv::Point laserStart = cv::Point(start, start);
    // cv::Point laserEnd = cv::Point(start+laserDist*cos(a*M_PI/180), start+laserDist*sin(a*M_PI/180));
    // if (a < 110 && a > 70) line(frame, laserStart, laserEnd, cv::Scalar_<float>(1., 0., 0.));
    // else line(frame, laserStart, laserEnd, cv::Scalar_<float>(0., 1., 0.));


    // alternative way of iterating through the line
    for(int i = 0; i < it.count; i++, ++it) {

      // If the ray goes outside of the image before finding a wall, give low weight
      if (it.pos().x >= 800.0 || it.pos().y >= 800.0) { 
        particleWeight += log(0.1);
        break;
      }

      float val = image.at<cv::Point3f>(it.pos()).x;

      // If the ray sees something outside the map that we don't know about, give it a lower weight
      if (val < 0) {
        particleWeight += log(0.1);
        break;
      }
      // If the ray hits a wall, then use the observation model
      else if (1 - val >= wallProb)
      {
        int rowDist = it.pos().y - startCell.row;
        int colDist = it.pos().x - startCell.col;
        
        float particleDistToWall = sqrt(rowDist*rowDist + colDist*colDist);
        float laserDistToWall = logLaserData[timestep].r[a]/10.0; // convert to grid cell units!
        if (laserDistToWall > 200.0)
        {
        	laserDistToWall = 200.0;
        }

        float p = pow(observationModel(laserDistToWall, particleDistToWall), 0.2);
   
        particleWeight += log(p);
        //particleWeight += log(p/(1-p));

        // Display the ray from the particle
        // if (a < 110 && a > 70) line(frame, cv::Point(startCell.col, startCell.row), it.pos(), cv::Scalar_<float>(1., 0., 0.));
        // else line(frame, cv::Point(startCell.col, startCell.row), it.pos(), cv::Scalar_<float>(0., 1., 0.));

        break;
      }
    }

    // Optionally display every ray and the end point that it compares with the laser data
    // cv::circle(frame, it.pos(), 4, cv::Scalar_<float>(0.,0.,1.), -1);
    // cv::namedWindow( "Wean Map", cv::WINDOW_AUTOSIZE);
    // if (!frame.empty()) {
    //   cv::imshow("Wean Map", frame);
    // }
    // cout << "Weight: " << particleWeight << " StartRow: " << startCell.row << " StartCol: " << startCell.col << " ItPosY: " << it.pos().y << " ItPosX: " << it.pos().x << endl;
    // cv::waitKey(10);

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

void ParticleFilter::updateWeightsCV(int timestep)
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

