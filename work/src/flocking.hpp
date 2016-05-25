#include “cgra_math.hpp

struct boid
{
	Geometry *model;
	vec2 position;
	vec2 direction;
	float speed; 
};

struct flock {
	
};

void update (boid *, flock *);

void render (boid *);
