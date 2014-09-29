#include <ParticleFilter.h>

#include <vector>
#include <stdlib.h>
#include <chrono>
#include <random>

using namespace std;

void ParticleFilter::drawParticles()
{
  for(int i = 0; i < 200; i++){
    int j = rand() % potentialParticles.size();
    particles.push_back(potentialParticles[j]);
  }
  return;
}

void ParticleFilter::motionModel()
{
	std::default_random_engine generator;
  std::normal_distribution<double> distribution(0.0,0.5);
  return;
}