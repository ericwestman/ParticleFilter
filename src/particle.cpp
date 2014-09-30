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
  for (int d = 1; d < 800; d++) 
  {
    Coord angCoord;
    angCoord.row = int(round(d*sin(angRad)));
    angCoord.col = int(round(d*cos(angRad)));
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

  while (angInDegree < -180) 
  {
    angInDegree += 360;
  }
  return angInDegree;
}


void ParticleFilter::buildRayCasterLUT()
{
  int angleIncr = 1;

  for (int i = -180; i <= 180; i += angleIncr) {
    RAY_CAST_LUT.push_back(castOneRay(i));
  }
}


vector<Coord> ParticleFilter::lookup(float robotTheta, int angle)
{
  // look up a vector of coordinates from the lookup table
  int normAngle = normalizeAngle(int(round(robotTheta)) + angle - 90);
  int angIdx = normAngle + 180;

  return RAY_CAST_LUT[angIdx];
}


float euclidDist(Coord start, Coord end) 
{
  return sqrt(pow((start.row - end.row),2) + pow((start.col - end.col),2));
}


float invGauss(float x, float mu, float sigma)
{
  return exp(-((mu - x) * (mu - x)) / ((sigma * sigma) * 2.0)) / (sqrt(2 * M_PI * (sigma * sigma)));
}


bool inGridBounds(Coord cell) 
{
  // make sure a cell is within the grid bounds
  return cell.row < 800 && cell.row > 0 && cell.col < 800 && cell.col > 0;
}



float ParticleFilter::calculateWeight_LUT(Particle &p, int timestep)
{
  float wallProb = 0.8;
  float laserError = 0.5;  // cm
  float accumWeights = 1;

  Coord startCell;
  startCell.row = min(int(round(p.getX())),800);
  startCell.col = min(int(round(p.getY())),800);


  for (int a = 0; a < 180; a++) 
  {
    vector<Coord> cells = lookup(p.getTheta()*180/M_PI, a);     
    for (int i = 0; i < cells.size(); i++)
    {
      Coord probCell;
      probCell.row = startCell.row + cells[i].row;
      probCell.col = startCell.col + cells[i].col;

      // if the ray hits a wall
      if (inGridBounds(probCell))
      {
        if (weanMap.prob[probCell.row][probCell.col] > wallProb)
        {
            float particleDistToWall = euclidDist(startCell,cells[i]);
            float actualDistToWall = logLaserData[timestep].r[a];
            accumWeights *= invGauss(particleDistToWall,actualDistToWall,laserError);
            break;
        }

        // we should probably build the actual weight distribution from class

        // if the ray never hits a wall but the actual laser does, give it some small weight 
        else if (i == cells.size()-1 && logLaserData[timestep].r[a] <= 8180)
        {
            accumWeights *= 0.001;
            break;
        }
        // if the ray never hits a wall and neither does the actual laser, give it some larger weight 
        else if (i == cells.size()-1 && logLaserData[timestep].r[a] > 8180)
        {
            accumWeights *= 0.01;
            break;
        }
      }
    }
  }


  return accumWeights;
}




float ParticleFilter::calculateWeight(Particle &p, int timestep) {
  float wallProb = 0.8;
  float laserError = 0.5;  // cm
  float accumWeights = 1;


  Coord startCell;
  startCell.row = min(int(round(p.getX())),800);
  startCell.col = min(int(round(p.getY())),800);


  for (int a = 0; a < 180; a++) 
  {   
    for (int i = 0; i < 800; i+=1)
    {
      float rowDist = sin(p.getTheta())*i;
      float colDist = cos(p.getTheta())*i;

      Coord cell;
      cell.row = int(round(rowDist+p.getY()));
      cell.col = int(round(colDist+p.getX()));


      if (inGridBounds(cell))
      {  
        // if the ray hits a wall
        if (weanMap.prob[cell.row][cell.col] >= wallProb)
        {
            float particleDistToWall = sqrt(rowDist*rowDist + colDist*colDist);
            float actualDistToWall = logLaserData[timestep].r[a];
            accumWeights *= invGauss(particleDistToWall,actualDistToWall,laserError);
            break;
        }
        // have to decide what to do here
        else if (weanMap.prob[cell.row][cell.col] == -1)
        {}
      }
      // and here...
      else
      {}
    }
  }
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



void ParticleFilter::updateWeights_LUT(int timestep)
{
  weights.clear();
  intervals.clear();

  for (int p = 0; p <= particles.size(); p++) {
    weights.push_back(calculateWeight_LUT(particles[p],timestep));
    intervals.push_back(p);
  }

  return;
}


void ParticleFilter::updateWeights_noLUT(int timestep)
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
      // Sanity check
      //assert(intervals[0] <= r && r <= *(end(intervals)-2));
      // Push the new particle into the vector
      particles.push_back(oldParticles[r]);
  }

  return;
}

