#pragma once

struct MyMap{
  int size_x, size_y, min_x, min_y, max_x, max_y;
  int resolution, global_mapsize_x, global_mapsize_y, offset_x, offset_y;
  float prob[800][800];
};

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


class MapCoord {
public:
  MapCoord() {};

  MapCoord(float x_in, float y_in) :
    x(x_in), 
    y(y_in) {};

  MapCoord(int r_in, int c_in) :
    x(c_in), 
    y(800 - r_in) {};

  float x, y;
};

class ImageCoord {
public:
  ImageCoord() {};

  ImageCoord(MapCoord m) :
    row(800 - m.y),
    col(m.x) {};
  int row, col;
};


class Particle {
public:
  Particle(float x_val, float y_val, float theta_val) :
    loc(x_val, y_val),
    theta(theta_val) {};

  MapCoord getLoc() {return this->loc;}

  float getX() {return this->loc.x;}

  float getY() {return this->loc.y;}

  float getTheta() {return this->theta;}

  void setX(int x_in) {this->loc.x = x_in; return;}

  void setY(int y_in) {this->loc.y = y_in; return;}

  void setTheta(double theta_in) {this->theta = theta_in; return;}

private:
  MapCoord loc;
  float theta;
};

