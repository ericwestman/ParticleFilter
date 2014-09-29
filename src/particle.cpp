#include <particle.h>

vector<Particle> drawParticles(vector<Particle>& potentialParticles){
	vector<Particle> v;
    for(int i = 0; i < 200; i++){
      int j = rand() % potentialParticles.size();
      v.push_back(potentialParticles[j]);
    }
  return v;
}

