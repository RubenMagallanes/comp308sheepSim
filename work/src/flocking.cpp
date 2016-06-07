#include "cgra_math.hpp"
#include "flocking.hpp"
#include <iostream>
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
	////std::cout << "creating new boid" << std::endl;

	boid b = boid ();
	b.model = m;
	//b.position = cgra::vec2 (0, 0);	

	if (fl->id_index == 1)
		b.position = cgra::vec2(4,4);
	else 
		b.position = cgra::vec2(-4,-4);

	b.velocity = cgra::vec2 (0, 0);		//x,z coords
	b.id = fl->id_index++;				//each boid should have unique id
	//std::cout << "creating and adding boid with id:"<< b.id << std::endl;
	fl->members.push_back (b);
	////std::cout << "new boid created and added" << std::endl;
}

void /* tells each boid in flock $fl to update. */
update_all (flock *fl)
{
	//std::cout << "--- UPDATING ALL FLOCK --- " << std::endl;
	int i;
	for (i= 0; i< fl->members.size(); i++)
	{
		update(&(fl->members[i]), fl);
	}
	//std::cout << std::endl;
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
	//rotate

	b->model->renderGeometry();
	
	//draw forces affecting each boid 
	glPopMatrix();

}


void /* individual boid's update function */
update (boid *b, flock *fl)
{
	////std::cout << "updating boid" << std::endl;
	//std::cout << "updating boid id=" << b->id << std::endl;
	//std::cout << "boid pos: ["<<b->position.x <<", "<<b->position.y<<"]" << std::endl;
	//std::cout << "boid vel: ["<<b->velocity.x <<", "<<b->velocity.y<<"]" << std::endl;

	cgra::vec2 sep = seperation (b, fl);  // boids dont collide
	cgra::vec2 coh = cohesion	(b, fl); // boids like to stick in packs
	cgra::vec2 ali = alignment	(b, fl); // boids like to steer in same dir

	// std::cout << "forces of boid: " << std::endl;
	// std::cout << "velocity before " << cgra::length(b->velocity) << std::endl;
	// std::cout << "seperation force: " << cgra::length(sep) << std::endl;
	// std::cout << "cohesion force: " << cgra::length(coh) << std::endl;
	// std::cout << "alignment force: " << cgra::length(ali) << std::endl;
	
	b->velocity += sep;
	b->velocity += coh; 
	b->velocity += ali;

	
	//impose speed restrictions here
	//if (cgra::length(b->velocity) > )
	// std::cout << "speed: " << cgra::length(b->velocity) << std::endl;
	if (cgra::length (b->velocity) > MAX_SPEED)
	{
			cgra::normalize(b->velocity);
			b->velocity *= MAX_SPEED; 	 // clamp speed to max value
	}
	b->position += b->velocity;

	std::cout << "velocity " << cgra::length(b->velocity) << std::endl;
	// std::cout << std::endl;

	if (cgra::length(b->position) > 25) // for testing, remove this later
	{
		b->position = cgra::normalize(b->position);

	}
	//std::cout << "update successfull " << std::endl;
	//std::cout <<  std::endl;
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

}