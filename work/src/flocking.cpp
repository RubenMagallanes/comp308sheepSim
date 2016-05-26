#include "cgra_math.hpp"
#include "flocking.hpp"

/*  ~  FLOCK FUNCTIONS  ~  */

void /* initialise flock $fl with $amt boids, each using $model as their geometry */
init_flock (flock *fl, int amt, Geometry* model)
{
	int i; 
	for (i= 0; i< amt; i++)
	{
		add_boid (fl, model);
	}
}

void /* create and add a boid using model $m to flock $fl */
add_boid (flock *fl, Geometry *m)
{
	boid b = boid ();
	b.model = m;
	b.position = vec2 (0, 0);		
	b.direction = vec2 (0, 1);		//x,z coords
	b.speed = 1;
	fl->members.push_back (b);
}

void /* tells each boid in flock $fl to update. */
update_all (flock *fl)
{
	int i;
	for (i= 0; i< fl->members.size(); i++)
	{
		update(fl->members[i], fl);
	}
}

/*  ~  BOID FUNCTIONS  ~  */

void 
update (boid *b, flock *fl)
{
	//
}

void 
render (boid *b)
{
	//todo translate and rotate
	b->model.renderGeometry();
}
