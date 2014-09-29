#pragma once
#include <vector>

struct Particle {
	int x, y;
	float theta;
};

std::vector<Particle> drawParticles(std::vector<Particle>& potentialParticles);