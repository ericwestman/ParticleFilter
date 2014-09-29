#include <ParticleFilter.h>

#include <vector>
#include <math.h>
#include <stdlib.h>

#define TWO_PI 6.2831853071795864769252866

using namespace std;
 
double generateGaussianNoise(const double &variance)
{
  static bool haveSpare = false;
  static double rand1, rand2;
 
  if(haveSpare)
  {
    haveSpare = false;
    return sqrt(variance * rand1) * sin(rand2);
  }
 
  haveSpare = true;
 
  rand1 = rand() / ((double) RAND_MAX);
  if(rand1 < 1e-100) rand1 = 1e-100;
  rand1 = -2 * log(rand1);
  rand2 = (rand() / ((double) RAND_MAX)) * TWO_PI;
 
  return sqrt(variance * rand1) * cos(rand2);
}

void ParticleFilter::drawParticles(int n)
{
  // Set up random number generator for heading
  std::random_device rd;
  std::mt19937_64 mt(rd());
  std::uniform_real_distribution<double> heading(0, TWO_PI);

  for(int i = 0; i < n; i++){
    // Get random particle from map
    int j = rand() % potentialParticles.size();

    // Assign random heading to particle
    Particle p = potentialParticles[j];
    p.setTheta(heading(mt));

    particles.push_back(p);
  }
  return;
}

void ParticleFilter::motionModel(int timestep)
{
	for (int i = 0; i < particles.size(); i++) {
	// 	int xrand = (int) 3*xDistribution(generator);
	// 	int yrand = (int) 3*yDistribution(generator);
    int xrand = (int) generateGaussianNoise(10);
    int yrand = (int) generateGaussianNoise(10);
		int x = particles[i].getX() + logOdometryData[timestep].x - logOdometryData[timestep - 1].x 
												 - xrand;
		int y = particles[i].getY() + logOdometryData[timestep].y - logOdometryData[timestep - 1].y 
												 - yrand;
		particles[i] = Particle(x, y, 0.0);
	}
  return;
}

void ParticleFilter::calculateWeights()
{
  return;
}

void ParticleFilter::resampleParticles()
{
  return;
}

