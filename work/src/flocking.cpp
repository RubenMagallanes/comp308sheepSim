#include "cgra_math.hpp"
#include "flocking.hpp"

/*  ~  FLOCK FUNCTIONS  ~  */

void /* initialise flock $fl with $amt boids, each using $model as their geometry */
init_flock (flock *fl, int amt, Geometry* model)
{
	fl->id_index = 0; // init id index
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
	b.position = cgra::vec2 (0, 0);		
	b.velocity = cgra::vec2 (0, 1);		//x,z coords
	b.id = fl->id_index++;				//each boid should have unique id

	fl->members.push_back (b);
}

void /* tells each boid in flock $fl to update. */
update_all (flock *fl)
{
	int i;
	for (i= 0; i< fl->members.size(); i++)
	{
		update(&(fl->members[i]), fl);
	}
}

void
render_all (flock *fl)
{
	int i;
	for (i= 0; i< fl->members.size(); i++)
	{
		render (&(fl->members[i]));
	}
}

/*  ~  BOID FUNCTIONS  ~  */


void 
render (boid *b)
{
	//todo translate and rotate
	glPushMatrix();
	glTranslatef(b->position.x, 0.0, b->position.y);
	
	b->model->renderGeometry();
	glPopMatrix();

}


void 
update (boid *b, flock *fl)
{
	cgra::vec2 sep = seperation (b, fl);
	cgra::vec2 coh = cohesion	(b, fl);
	cgra::vec2 ali = alignment	(b, fl);

	b->velocity += sep;
	b->velocity += coh; 
	b->velocity += ali;

	//impose speed restrictions here
	b->position += b->velocity;
}
//----------- helper functions for update
/*return seperation vector*/
cgra::vec2 seperation(boid *b, flock *fl){
	

		return cgra::vec2(0,0);

}
cgra::vec2 cohesion(boid *b, flock *fl){
			return cgra::vec2(0,0);
}
cgra::vec2 alignment(boid *b, flock *fl){
			return cgra::vec2(0,0);

}