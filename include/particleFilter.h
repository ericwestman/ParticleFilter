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
  void drawParticles(int n);
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
  std::vector<Particle> potentialParticles;
  std::vector<Particle> particles;
  std::vector<double> weights;

  // Data pertaining to log file
  char *logName;
  std::vector<LaserData> logLaserData;
  std::vector<OdometryData> logOdometryData;

  // Define probability distributions
  std::default_random_engine generator;
  std::normal_distribution<double> xDistribution;
  std::normal_distribution<double> yDistribution;
};