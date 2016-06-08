#include "cgra_math.hpp"
#include "flocking.hpp"
#include "cgra_geometry.hpp"
#include <iostream>

/*  ~  FLOCK FUNCTIONS  ~  */

void /* initialise flock $fl with each boid using $model as their geometry */
init_flock (flock *fl,  Geometry *model_)
{
	fl->id_index = 0;  
	fl->model = model_; 
}

void /* create and add a boid at position $x and $y to flock $fl */
add_boid (flock *fl, float x_, float y_)
{
	boid b = boid ();
	b.model = fl->model;
	b.position = cgra::vec2 (x_, y_);	

	b.velocity = cgra::vec2 (0, 0.1);		
	b.id = fl->id_index++;				//each boid should have unique id
	b.rotation = 0;
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

/*  ~  INDIVIDUAL BOID FUNCTIONS  ~  */


void 
render (boid *b)
{
	glPushMatrix();
	glTranslatef(b->position.x, 0.0, b->position.y); // z value based on terrain 

	// rotate around y axis (-1 because vectors around wrong way)
	glRotatef (b->rotation , 0, -1, 0); 
	
	b->model->renderGeometry();

	glTranslatef (0.0, 1.7, 0.0);
	float velveclen =0.1+ cgra::length(b->velocity) * 10;
	cgra::cgraCylinder (0.15, 0.0, velveclen);
	//draw forces affecting each boid 
	glPopMatrix();

}


void /* individual boid's update function */
update (boid *b, flock *fl)
{
	/* CALCULATE AFFECTORS */
	cgra::vec2 sep = seperation (b, fl); // boids dont collide 
	//TODO perhaps add weak seperation. current sep values prevent collisions 
	cgra::vec2 coh = cohesion	(b, fl); // boids like to stick in packs
	cgra::vec2 ali = alignment	(b, fl); // boids like to steer in same dir

	/* ADD AFFECTORS TO BOID'S VELOCITY */
	b->velocity += sep;
	b->velocity += coh;
	b->velocity += ali;

	/* clamp to $MAX_SPEED velocity */
	b->velocity = cgra::clamp (b->velocity, -MAX_SPEED, MAX_SPEED);

	/* UPDATE BOID'S ROTATION for rendering *///maybe in seperate function?
	if (cgra::length(b->velocity) != 0) // no divide by zero error
	{
		cgra::vec2 z , v;
		z = cgra::vec2 (0, 1); 				// normalised z axis
	 	v = cgra::normalize (b->velocity);  // normalzed velocity

		float dp = cgra::dot (z, v); 		//dot product
		float cp = z.x* v.y - z.y*v.x; 		//cross
		float theta = std::atan2 (cp, dp);
		theta = theta * (180/cgra::math::pi()); // convert to degrees
		b->rotation = theta;				// updates rotation value
	}

	/* UPDATE POSITION BASED ON VELOCITY */
	b->position += b->velocity;

	//std::cout << "velocity " << cgra::length(b->velocity) << std::endl;
	// std::cout << std::endl;

	if (cgra::length(b->position) > 25) // for testing, remove this later when hay implemented
	{
		b->position = cgra::normalize(b->position);

	}
}
//----------- helper functions for update
void //TODO change to not return the current boid , this saves us a check in each s, c ,a vector
construct_flock_list (std::vector<boid *> *list, flock * fl, boid *current, char caller)
{
	//take flock, add to list of boid pointers depending on calling function
	//initially, add all. later change this to only withinn a threshold
	int i; 
	for (i=0; i< fl->members.size(); i++)
	{
		if (fl->members[i].id != current->id) // onyl add if not this one
			list->push_back(&(fl->members[i]));	
	}
	
}

/*return seperation vector*/
cgra::vec2 
seperation(boid *current, flock *fl){
	int n=0; 							
	cgra::vec2 seperation_force = cgra::vec2 (0,0); // force vector to be returned
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, current, 's');// flock is now full of relevant boids

	for (boid *other : flock)
	{
		if (cgra::distance (current->position, other->position) < SEPERATION_THRESHOLD)
		{
			/*force pushing away is stronger the closer this is to other*/
			float push_scalar = SEPERATION_THRESHOLD - cgra::distance (current->position, other->position);
			push_scalar *= SEPERATION_FACTOR; // scale by factor so it reasonable 
			/* vector to be returned  = this - other */
			cgra::vec2 push_vector = current->position - other->position;
			//change magnitude to be push_scalar length
			push_vector = push_vector = cgra::normalize(push_vector);
			push_vector *= push_scalar;

			seperation_force += push_vector; //add to one to be reurned
			n++;
		}
	}
	if (n==0)
	{	
		return cgra::vec2(0,0); 					//if no other boids, no effect
	}

	seperation_force /= n; // divide by no of vectors affecting this

	//return cgra::vec2(0, 0);
	return seperation_force; 
}

cgra::vec2 
cohesion(boid *current, flock *fl){
	int n=0; 									
	cgra::vec2 average_position = cgra::vec2 (0,0); 
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, current, 'c');
	// flock is now full of relevant boids

	for (boid *other : flock)						//iterate through each boid affecting this
	{
		
			average_position += other->position;	//add it's position to the average
			n++;
		
	}
	if (n==0)
	{	
		return cgra::vec2(0,0); 					//if no other boids, no effect
	}
 	
	average_position /= n;							 // divide vector size by number of other boids
	//average position is now correct; 
		
	float factor = (COHESION_FACTOR) / 100.0f;		//factor is float from 0.01 to 100, explained in flocking.hpp
													//return vector pointing at average_pos scaled by $factor
	//(average_pos - curr->pos) is vector pointing towards ave from curr
	//it's magnitude needs to be scaled by a $factor so it only moves a small percent of the way towards the 
	//ave position. 
	cgra::vec2 ret = (average_position - current->position) * factor;
	return ret;
}

cgra::vec2 
alignment(boid *current, flock *fl){
	//similar to cohesion, but we average velocities, and take a small portion of the perceived velocity
	//and return 
	return cgra::vec2(0,0);

	int n=0;
	cgra::vec2 average_velocity = cgra::vec2 (0,0); 
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, current, 'a');

	for (boid *other : flock)						//iterate through each boid affecting this
	{
		average_velocity += other->velocity;	//add it's position to the average
		n++;
	}
	if (n==0)
	{	
		return cgra::vec2(0,0); 					//if no other boids, no effect
	}
	average_velocity /= n;							//divide by num of velocities
	float factor = (ALIGNMENT_FACTOR) / 100.0f;		//how much of ave vel to return
	cgra::vec2 ret = average_velocity * factor;  	//scale
	return ret; 
}
/*
~
~
~
~
~
~	
~
~
~
~
~
~
*/