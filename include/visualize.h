// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
#pragma once

#include <opencv2/opencv.hpp>
#include <mapReader.h>
#include <iostream>
#include <particle.h>
#include <stdlib.h>
#include <vector>

void visualize(MyMap *occupancyMap, std::vector<Particle>& particles);

void dispParticles(cv::Mat &image, std::vector<Particle> &samples);