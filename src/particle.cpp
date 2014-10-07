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


  	// video 3 and 1
	//Particle p = Particle(395.0,400.0,-M_PI/2);

	// video 4
    //Particle p = Particle(485.0,400.0,M_PI/2);

    // Assign random heading to particle
    p.setTheta(heading(mt));
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
  float sigma_g = 10;

  float g_max = 1.0;
  float gaussian = g_max*exp(-((mu - x) * (mu - x)) / ((sigma_g * sigma_g) * 2.0));

  // Max range component
  float max_range_prob = g_max/3; //Jane
  float max_range = (x > 818.0) ? max_range_prob : 0.0;

  // Uniform component
  float uniform = g_max * 1.0/4.0;

  // Exponential component
  float sigma_e = -0.0008;
  float a = g_max*1.0/3.0;
  float exponential = a*exp(sigma_e*x);

  return gaussian + exponential + uniform + max_range;
}

float ParticleFilter::calculateWeight(Particle &p, int timestep) {
  float wallProb = 0.55;
  float particleWeight = 0.0;

  int laserRange = 400;

  ImageCoord startCell = ImageCoord(p.getLoc());

  for (int a = 0; a < 180; a+=4) {

    ImageCoord endCell = ImageCoord();
    endCell.row = 800 - int(round(p.getX()+laserRange*cos(p.getTheta()+(a-90)*M_PI/180)));
    endCell.col = int(round(p.getY()+laserRange*sin(p.getTheta()+(a-90)*M_PI/180)));
    
    // grabs pixels along the line (pt1, pt2)
    // from 8-bit 3-channel image to the buffer
    cv::LineIterator it(image, cv::Point(startCell.col, startCell.row), 
                               cv::Point(endCell.col, endCell.row), 8);


    // alternative way of iterating through the line
    for(int i = 0; i < it.count; i++, ++it) {

      // If the ray goes outside of the image before finding a wall, give low weight
      if (it.pos().x >= 800.0 || it.pos().y >= 800.0) { 
        particleWeight += log(0.1);
        break;
      }

      float val = image.at<cv::Point3f>(it.pos()).x;

      // If the ray sees something outside the map that we don't know about, give it a lower weight
      // if (val < 0 && (logLaserData[timestep].r[a]/10.0 >= 818.0)) {
      //   particleWeight += log(1.1);
      //   break;
      // }
      // else if (val < 0 && (logLaserData[timestep].r[a]/10.0 < 818.0)) {
      // 	particleWeight += log(0.001);
      // 	break;
      // }	

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
        if (laserDistToWall > laserRange*1.0)
        {
        	laserDistToWall = laserRange*1.0;
        }

        float p = pow(observationModel(laserDistToWall, particleDistToWall), 0.2);
   
        particleWeight += log(p);

        break;
      }
    }

  }

  return exp(particleWeight);
}


void ParticleFilter::updateWeights(int timestep)
{
  weights.clear();
  intervals.clear();

  for (int p = 0; p <= particles.size(); p++) {
    weights.push_back(calculateWeight(particles[p],timestep));
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

float normalizeAngle(float &angInRad) 
{
  // puts degree in range -180 to 180
  while (angInRad > M_PI) 
  {
    angInRad -= 2*M_PI;
  }

  while (angInRad < -M_PI) 
  {
    angInRad += 2*M_PI;
  }
  return angInRad;
}




void ParticleFilter::normalizeWeights()
{
	float weightTotal = 0;
	for (int i = 0; i < weights.size(); i++) {
		weightTotal += weights[i];
	}

	for (int i = 0; i < weights.size(); i++) {
		weights[i] /= weightTotal;
	}
	return;
}


void ParticleFilter::estimatePosition_average()
// estimate position by taking mean of particle positions
{
	float estX = 0;
	float estY = 0;
	float sumSin = 0;
	float sumCos = 0;
	float estTheta = 0;
	for (int i = 0; i < particles.size(); i++) {
		estX += particles[i].getX();
		estY += particles[i].getY();

		sumSin += sin(particles[i].getTheta());
		sumCos += cos(particles[i].getTheta());
	}

	estX = estX/particles.size();
	estY = estY/particles.size();
	estTheta = atan2(sumSin/particles.size(),sumCos/particles.size());
	normalizeAngle(estTheta);

	estPosition.setX(estX);
	estPosition.setY(estY);
	estPosition.setTheta(estTheta);

}

void ParticleFilter::estimatePosition_maxWeight()
// estimate position as the position of the highest weighted particle
{

	float maxWeight = 0;
	int maxIdx = 0;
	for (int i = 0; i < weights.size(); i++) {
		if (weights[i] > maxWeight) {
			maxWeight = weights[i];
			maxIdx = i;
		}
	}

	estPosition.setX(particles[maxIdx].getX());
	estPosition.setY(particles[maxIdx].getY());
	estPosition.setTheta(particles[maxIdx].getTheta());

	return;

}


void ParticleFilter::estimatePosition_weightedAverage()
// estimate position as weighted mean based on each particle's weight
{
	float estX = 0;
	float estY = 0;
	float sumSin = 0;
	float sumCos = 0;
	float estTheta = 0;

	normalizeWeights();

	for (int i = 0; i < particles.size(); i++) {
		estX += particles[i].getX()*weights[i];
		estY += particles[i].getY()*weights[i];

		sumSin += sin(particles[i].getTheta());
		sumCos += cos(particles[i].getTheta());
	}

	// can't just take mean of angles, need to do it this way	
	estTheta = atan2(sumSin/particles.size(),sumCos/particles.size());
	normalizeAngle(estTheta);

	estPosition.setX(estX);
	estPosition.setY(estY);
	estPosition.setTheta(estTheta);

}
