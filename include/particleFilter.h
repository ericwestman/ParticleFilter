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
	int readMap();
	void readLog();
  void drawParticles();
	void redrawParticles();
  void motionModel(int timestep);

  // Visualize functions
  void dispParticles();
  void loadMapImage();
  void visualize();

  std::vector<float> timestamps;

private:
  // Data pertaining to map
  char *weanMapName;
  MyMap weanMap;
  std::vector<Particle> potentialParticles;
  std::vector<Particle> particles;
  cv::Mat image;
  cv::Mat frame;

  // Data pertaining to log file
  char *logName;
  std::vector<LaserData> logLaserData;
  std::vector<OdometryData> logOdometryData;

  // Define probability distributions
  std::default_random_engine generator;
  std::normal_distribution<double> xDistribution;
  std::normal_distribution<double> yDistribution;
};