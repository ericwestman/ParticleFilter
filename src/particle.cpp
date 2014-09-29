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
		int xrand= (int) 3*xDistribution(generator);
		int yrand= (int) 3*yDistribution(generator);
		int x = particles[i].getX() + logOdometryData[timestep].x - logOdometryData[timestep - 1].x 
												 - xrand;
		int y = particles[i].getY() + logOdometryData[timestep].y - logOdometryData[timestep - 1].y 
												 - yrand;
		particles[i] = Particle(x, y, 0.0);
		cout << xrand << ", " << yrand << endl;
	}
  return;
}