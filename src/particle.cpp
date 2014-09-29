#include <ParticleFilter.h>

#include <vector>
#include <stdlib.h>

using namespace std;

void ParticleFilter::drawParticles()
{
  for(int i = 0; i < 200; i++){
    int j = rand() % potentialParticles.size();
    particles.push_back(potentialParticles[j]);
  }
  return;
}

void ParticleFilter::redrawParticles()
{
  for(int i = 0; i < 200; i++){
    int j = rand() % potentialParticles.size();
    particles[i] = (potentialParticles[j]);
  }
  return;
}

void ParticleFilter::motionModel(int timestep)
{
	for (int i = 0; i < particles.size(); i++) {
		Particle p;
		int xrand= (int) xDistribution(generator);
		int yrand= (int) yDistribution(generator);
		p.x = particles[i].x + logOdometryData[timestep].x - logOdometryData[timestep - 1].x 
												 - xrand;
		p.y = particles[i].y + logOdometryData[timestep].y - logOdometryData[timestep - 1].y 
												 - yrand;
		p.theta = 0;
		particles[i] = p;
		cout << p.x << ", " << p.y << endl;
	}
  return;
}