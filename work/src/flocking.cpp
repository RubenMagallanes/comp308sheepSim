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
		b.position = cgra::vec2(2,2);
	else 
		b.position = cgra::vec2(-2,-2);

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

	b->model->renderGeometry();
	glPopMatrix();

}


void 
update (boid *b, flock *fl)
{
	////std::cout << "updating boid" << std::endl;
	//std::cout << "updating boid id=" << b->id << std::endl;
	//std::cout << "boid pos: ["<<b->position.x <<", "<<b->position.y<<"]" << std::endl;
	//std::cout << "boid vel: ["<<b->velocity.x <<", "<<b->velocity.y<<"]" << std::endl;

	cgra::vec2 sep = seperation (b, fl); // boids dont collide
	cgra::vec2 coh = cohesion	(b, fl); // boids like to stick in packs
	cgra::vec2 ali = alignment	(b, fl); // boids like to steer in same dir

	b->velocity += sep;
	b->velocity += coh; 
	b->velocity += ali;

	//impose speed restrictions here
	b->position += b->velocity;
	//std::cout << "update successfull " << std::endl;
	//std::cout <<  std::endl;
}
//----------- helper functions for update
void 
construct_flock_list (std::vector<boid *> *list, flock * fl, boid *current, char caller)
{
	//take flock, add to list of boid pointers depending on calling function
	//initially, add all. later change this to only withinn a threshold
	//std::cout << "in construct_flock_list"<< std::endl;
	int i; 
	for (i=0; i< fl->members.size(); i++)//(boid b : fl->members)
	{

		////std::cout << "adding boid to flock_list id="<< b.id << std::endl;
		list->push_back(&(fl->members[i]));	// add address to list

	}
	////std::cout << "flock_list created successfully" << std::endl;
}

/*return seperation vector*/
cgra::vec2 
seperation(boid *b, flock *fl){
	

		return cgra::vec2(0,0);

}

cgra::vec2 
cohesion(boid *current, flock *fl){
	//std::cout << "attempting cohesion calculation for boid id="<< current->id<< std::endl;
	int n=0; 									
	cgra::vec2 average_position = cgra::vec2 (0,0); // force vector to be returned
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, current, 'c');// flock is now full of relevant boids

	////std::cout << " flock_list_contains: " << std::endl;


	for (boid *other : flock)
	{
		//std::cout << "comparing against other boid id=" << other->id << std::endl;
		if (other->id != current->id) 				// if not this one
		{
			//std::cout << "other position ["<<other->position.x<<", "<<other->position.y<<"]" << std::endl;
			average_position += other->position;	//add it's position to the average
			n++;
		}
	}
	if (n==0)
	{	
		//std::cout << " zero " << std::endl;
		return cgra::vec2(0,0); 					//if no other boids, no effect
	}
 	////std::cout << "average position ["<< average_position.x <<", "<< average_position.y<<"]" << std::endl;
	
	average_position /= n;		// divide vector size by number of other boids
	//std::cout << "average position divided by "<< n << " ["<< average_position.x <<", "<< average_position.y<<"]" << std::endl;
	
	//float factor = 50.0f / 100.0f;		
	float factor = (COHESION_FACTOR) / 100.0f;			//factor is float from 0.01 to 1, explained in flocking.hpp
													//return vector pointing at average_pos scaled by $factor
	//std::cout << "factor " << factor << std::endl;

	cgra::vec2 ret = (average_position - current->position) * factor;
	////std::cout << "vector in direction of average center ["<< ret.x << ", " << ret.y <<"]"<< std::endl;
	

	//std::cout << "cohesion successful, returning vector: [" << ret.x << ", " << ret.y << "]" << std::endl;
	return (average_position - current->position) * factor;
}

cgra::vec2 
alignment(boid *b, flock *fl){
			return cgra::vec2(0,0);

}