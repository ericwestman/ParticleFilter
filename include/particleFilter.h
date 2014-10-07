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
  void drawTestParticles();
  void motionModel(int timestep);
  float calculateWeight_LUT(Particle &p, int timestep);
  float calculateWeight(Particle &p, int timestep);
  float calculateWeightCV(Particle &p, int timestep);
  void updateWeights_test();
  void updateWeights_LUT(int timestep);
  void updateWeights(int timestep);
  void updateWeightsCV(int timestep);
  void resampleParticles();
  void estimatePosition_average();
  void estimatePosition_weightedAverage();
  void estimatePosition_maxWeight();
  void normalizeWeights();

  // Visualize methods
  void dispParticles();
  void dispAllParticles();
  void dispTestParticles();
  void dispLasers(int &timestep);
  void loadMapImage();
  void writeVideo();
  void visualize();
  void visualizeWithRays(int timestep);
  void visualizeTestParticles();

  std::vector<float> timestamps;

private:
  // Data pertaining to map
  char *weanMapName;
  MyMap weanMap;
  cv::Mat image;
  cv::Mat frame;

  // Particle data
  int numParticles;
  int numTestParticles;
  std::vector<Particle> potentialParticles;
  std::vector<Particle> particles;
  std::vector<float> weights;
  std::vector<float> weightedDistribution;
  std::vector<int> intervals;
  Particle estPosition = Particle(395.0,400.0,M_PI/2);

  // Data pertaining to log file and video file
  char *logName;
  char *videoName;
  cv::VideoWriter outputVideo;
  std::vector<LaserData> logLaserData;
  std::vector<OdometryData> logOdometryData;

  // Setup for probability distributions
  unsigned seed;
  std::default_random_engine generator;
  std::normal_distribution<double> xy_normal;
  std::normal_distribution<double> theta_normal;



};