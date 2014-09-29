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
  void motionModel(int timestep);
  void calculateWeights();
  void resampleParticles();

  // Visualize methods
  void dispParticles();
  void loadMapImage();
  void visualize();

  std::vector<float> timestamps;

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
  std::normal_distribution<double> normal;
};