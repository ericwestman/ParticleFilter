#pragma once

#include <types.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <random>

class ParticleFilter {
public:
  ParticleFilter();
  // File I/O methods
	int readMap();
	void readLog();

  // Particle filter methods
  void drawParticles();
  void motionModel(int &timestep);
  float calculateWeight_LUT(Particle &p, int &timestep);
  float calculateWeight(Particle &p, int &timestep);
  float calculateWeightCV(Particle &p, int &timestep);
  void updateWeights_test();
  void updateWeights_LUT(int &timestep);
  void updateWeights(int &timestep);
  void updateWeightsCV(int &timestep);
  void resampleParticles();

  // Visualize methods
  void dispParticles();
  void loadMapImage();
  void visualize();

  std::vector<float> timestamps;

  // ray caster lookup table
  void buildRayCasterLUT();
  std::vector<Coord> lookup(int &robotTheta, int &angle);
  std::vector<std::vector<Coord>> RAY_CAST_LUT;

private:
  // Data pertaining to map
  char *weanMapName;
  MyMap weanMap;
  cv::Mat image;
  cv::Mat frame;

  // Particle data
  int numParticles;
  std::vector<Particle> potentialParticles;
  std::vector<Particle> particles;
  std::vector<float> weights;
  std::vector<float> weightedDistribution;
  std::vector<int> intervals;

  // Data pertaining to log file
  char *logName;
  std::vector<LaserData> logLaserData;
  std::vector<OdometryData> logOdometryData;

  // Setup for probability distributions
  unsigned seed;
  std::default_random_engine generator;
  std::normal_distribution<double> xy_normal;
  std::normal_distribution<double> theta_normal;



};