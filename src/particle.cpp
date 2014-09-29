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
  std::uniform_real_distribution<float> heading(0, TWO_PI);

  for(int i = 0; i < numParticles; ++i){
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
	for (int i = 0; i < particles.size(); ++i) {
		int xrand = (int) normal(generator);
		int yrand = (int) normal(generator);
		int x = particles[i].getX() + logOdometryData[timestep].x - logOdometryData[timestep - 1].x 
												 - xrand;
		int y = particles[i].getY() + logOdometryData[timestep].y - logOdometryData[timestep - 1].y 
												 - yrand;
		particles[i] = Particle(x, y, 0.0);
	}
  return;
}

float ParticleFilter::calculateWeight()
{
  return 0.0;
}

void ParticleFilter::updateWeights()
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
      // Sanity check
      assert(intervals[0] <= r && r <= *(end(intervals)-2));
      // Push the new particle into the vector
      particles.push_back(oldParticles[r]);
  }

  return;
}

