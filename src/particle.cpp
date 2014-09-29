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


vector<Coord> castOneRay(int angle) 
{
  vector<Coord> ray;

  float angRad = angle*M_PI/180;
  for (int d = 1; d++; d < 800) 
  {
    Coord angCoord;
    angCoord.row = round(d*sin(angRad));
    angCoord.col = round(d*cos(angRad));
    ray.push_back(angCoord);
  }
  return ray;
}


int normalizeAngle(int angInDegree) 
{
  // puts degree in range -180 to 180
  while (angInDegree > 180) 
  {
    angInDegree -= 360;
  }

  while (angInDegre < -180) 
  {
    angInDegree += 360;
  }
}


vector<vector<Coord>> ParticleFilter::buildRayCasterLUT()
{
  int angleIncr = 1;

  for (int i = -180; i+=angleIncr; i<=180) {
    RAY_CAST_LUT.push_back(castOneRay(i))
  }
  return RAY_CAST_LUT;
}


vector<Coord> lookup(float robotTheta, int angle)
{
  int normAng = normalizeAngle(round(robotTheta) + angle - 90);
  int angIdx = normAngle + 180;

  vector<Coord> cells;
  cells = RAY_CAST_LUT[angIdx];

}


float ParticleFilter::calculateWeight(Particle &p, int timestep)
{
  Coord startCell;
  startCell.row = round(p.getX()) + 0.5;
  startCell.col = round(p.getY()) + 0.5;

  for (int a = 0; a++; a<=180) 
  {
    vector<Coord> cells = lookup(p.getTheta(),a)
    for (int i = 0; i++; cells.size())
    {
      // getting wieghts and shit
     // if prob(cells[i].row + cell.ro
    }
  }

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

