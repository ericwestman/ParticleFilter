#include <logReader.h>

using namespace std;

vector<string>& split(const string &s, char delim, vector<string> &elems) 
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) 
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

void read_log(char *logName, vector<LaserData> *logLaser, 
                             vector<OdometryData> *logOdometry)
{
  string line;
  ifstream myfile (logName);
  if (myfile.is_open()) {
    while ( getline (myfile,line) ) {
      vector<string> elements = split(line, ' ');
      if (elements[0] == "O") {
      	OdometryData odom;
      	odom.x 			= stof(elements[1]);
      	odom.y 			= stof(elements[2]);
      	odom.theta 	= stof(elements[3]);
      	odom.ts 		= stof(elements[4]);
      	logOdometry->push_back(odom);
      }
      else if (elements[0] == "L"){
      	LaserData laser;
      	laser.x = 			stof(elements[1]);
      	laser.y = 			stof(elements[2]);
      	laser.theta = 	stof(elements[3]);
      	laser.xl = 			stof(elements[4]);
      	laser.yl = 			stof(elements[5]);
      	laser.thetal =	stof(elements[6]);
      	laser.ts = 			stof(elements[187]);

      	for (int i = 0; i < 180; i ++){
      		laser.r[i] = stoi(elements[i+7]);
      	}
      }
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 
	return;
};

// This should read any of the log files in data/log/ and fill in two vectors:
// one for laser readings, and another for odometry readings
// This will involve defining structs for the laser and odometry measurements
// and some basic file reading