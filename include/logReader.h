#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

struct LaserData{
	// Robot pose
	float x, y, theta;
	// Laser Pose (approximately 25 cm offset forward from true center of robot)
	float xl, yl, thetal;
	// Range readings of laser in cm. 180 readings span 180 degrees 
	// starting right, going left
	int r[180];
	// Timestamp of laser reading
	float ts;
};

struct OdometryData{
  // Coordinates of robot in standard odometry frame
	float x, y, theta;
  // Timestamp of odometry reading
  float ts;
};

void read_log(char *logName, std::vector<LaserData> &logLaser, 
                             std::vector<OdometryData> &logOdometry,
                             std::vector<float> &timestamps);