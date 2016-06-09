#include "cgra_math.hpp"
#include "flocking.hpp"
#include "cgra_geometry.hpp"
#include <iostream>


static float f_sep_strength = 0.01f; // flocking seperation strength


static float f_max_velocity = 0.7f; // decent - 1.4

static float e_sep_strength = 0.02f; // eating sep strength bigger 

static float e_coh_strength = 0.3f; // v
static float eating_alignment_scalar = 0.3;
static float e_max_velocity = 0.2f; // slow  - 0.4

static float p_sep_strength = 1.0f; // panic
static float p_coh_strength = 0.3f; // v

static float p_max_velocity = 1.0f; // fast - 2.0


//static float sep_strength =  0.003f; // for actual seperation, not collision


static float e_DECEL = 0.001f; // eating lsows
static float f_DECEL = 0.0001f;
/*  ~  FLOCK FUNCTIONS  ~  */

void /* initialise flock $fl with each boid using $model as their geometry */
init_flock (flock *fl,  Geometry *model_, std::vector<affector> *aff_)
{
	fl->id_index = 0;  
	fl->model = model_; 
	fl->affectors = aff_;
}

void /* create and add a boid at position $x and $y to flock $fl */
add_boid (flock *fl, float x_, float y_, float vx_, float vy_)
{
	boid b = boid ();
	b.model = fl->model;
	b.position = cgra::vec2 (x_, y_);	

	
	b.velocity = cgra::vec2 (vx_, vy_);		
	b.id = fl->id_index++;				//each boid should have unique id
	b.rotation = 0;
	fl->members.push_back (b);
}

void /* creates an affector with type $type_, at position $x_, $y_, with model $geo_, saved in list $list_*/
create_affector (std::vector<affector> * list_, Geometry * geo_, int type_, float x_, float y_)
{
	affector a = affector ();
	a.model = geo_;
	a.position = cgra::vec2 (x_, y_);
	if (type_ == 1 || type_ == 2) // must match 
		a.type = type_;
	else 
		1 /0; 

	list_->push_back(a);
}

void
render_affectors(std::vector<affector> *affectors)
{
	int i;
	for (i= 0; i< affectors->size(); i++)
	{
		glPushMatrix();
		struct affector a = affectors->at(i);
		
		 glTranslatef(a.position.x, 1.0, a.position.y);//todo y value affected by terrain underneath
		 a.model->renderGeometry();
		glPopMatrix();
	}
}


/* 'all' functions */

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
	glTranslatef(b->position.x, 1.0, b->position.y); // z value based on terrain 

	// rotate around y axis (-1 because vectors around wrong way)
	glRotatef (b->rotation , 0, -1, 0); 
	
	b->model->renderGeometry();

	glTranslatef (0.0, 1.7, 0.0);
	float velveclen =0.1+ cgra::length(b->velocity) * 10;
	cgra::cgraCylinder (0.15, 0.0, velveclen);
	//draw forces affecting each boid 
	glPopMatrix();

}

int // returns other within 20
boids_around (boid *b, flock *fl)
{
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, b, 'b');// flock is now full of relevant boids
	int count = 0;
	for (boid *other : flock)
	{
		if (cgra::distance (other->position, b->position) < 20.0) // if within 20
			count ++; 
	}
	return count;

}

cgra::vec2 /* returns a random vector length not equal to zero */
rand_vector (float length){
	int neg =  0 + (rand() % 1); // 0 and 1
	float x =  (float)(1.0f + (rand() % 10))/10.0f ; // 0.1 - 1
	if (neg)
		x = -x; // flip 50% of the time

	neg =  0+ (rand() % 1); // 0 and 1
	float y =  (float)(1.0f + (rand() % 10))/10.0f; // 0.1 - 1
	if (neg)
		y = -y; // flip 50% of the time

	std::cout << x << ", " << y << std::endl;

	cgra::vec2 ret = cgra::vec2(x, y);
	ret = cgra::normalize (ret);
	ret *= length;

	
	return ret;
}


void 
check_state (boid *b, flock *fl)
{
	//std::cout << "state: " << b->state << std::endl;
	int in_view = boids_around (b, fl); // in close proximity
	//implement pred check first
	if (b->state == 'e')
	{
		if (in_view < 3) // not enough friends
		{ 
			b->lonely ++; 
			 // state transition
			if (b->lonely == 100) // alone for 1.6 seconds
			{
				std::cout << "state change to f" << std::endl;
				b->state = 'f'; // flock to find friends
				b->timer = 160; // wont stop flocking for at least 3 seconds
			}
		} else {
			b->bored ++;
			if(b->bored >= 700){ // wants to move
				b->state = 'r';
				b->bored =  0;
				std::cout << "state change to r" << std::endl;
				b->velocity = rand_vector(0.2);
				b->timer = 400; //runs for 200 tickss
			}
		}
	}
	if (b->state == 'f'){
		b->timer -- ; // 1 step closer to settling down
		if  (in_view >= 3 && b->timer == 0){
			std::cout << "state change to e" << std::endl;
			b->state = 'e'; // enough other around to eat
			b->lonely = 0;
		}
	}
	if (b->state == 'r')//running
	{
		b->timer -- ;
		if (b->timer == 0)
		{
			std::cout << "state change to e" << std::endl;
			b->state = 'e';
			b->bored = 0+ (rand() % 500); // randomize time to next boredom 0-500
		}
	}

}


void /* individual boid's update function */
update (boid *b, flock *fl)
{

	check_state (b, fl);
	/* CALCULATE AFFECTORS */
	cgra::vec2 sep1 = seperation (b, fl); // boids dont collide 
	//TODO perhaps add weak seperation. current sep values prevent collisions only
	//cgra::vec2 sep2 // actual seperation
	cgra::vec2 coh = cohesion	(b, fl); // boids like to stick in packs
	cgra::vec2 ali = alignment	(b, fl); // boids like to steer in same dir
	
	/*std::cout << "--------------" << std::endl; 
	std::cout << "strength of sep: " << cgra::length (sep) << std::endl; 
	std::cout << "strength of coh: " << cgra::length (coh) << std::endl; 
	std::cout << "strength of ali: " << cgra::length (ali) << std::endl; */

	cgra::vec2 hay = pull_to_hay (b, fl);//SHEEP LIKE HAY LUL
	/*std::cout << "strength of hay: " << cgra::length (hay) << std::endl; 
	
	std::cout << "velocity of boi: " << cgra::length (b->velocity) << std::endl;*/ 
	/* ADD AFFECTORS TO BOID'S VELOCITY */
	b->velocity += sep1;
	b->velocity += coh;
	b->velocity += ali;

	b->velocity += hay;


	//std::cout << "vel of boi afte: " << cgra::length (b->velocity) << std::endl; 

	/* clamp to $MAX_SPEED velocity */
	if (b->state == 'f' || b->state == 'r')
		b->velocity = cgra::clamp (b->velocity, -MAX_FLOCK_SPEED, MAX_FLOCK_SPEED);
	else if (b->state == 'e')
		b->velocity = cgra::clamp (b->velocity, -MAX_EAT_SPEED, MAX_EAT_SPEED);
		
	//decelerate depending on state
	float speed = cgra::length (b->velocity);
	if (b->state == 'e')
		speed -= e_DECEL; 
	else if (b->state == 'f')
		speed -= f_DECEL;
	cgra::vec2 b_vel = b->velocity;
	b_vel = cgra::normalize (b_vel);
	b_vel *= speed; 
	b->velocity = b_vel;

	//std::cout << "boi vel decel : " << cgra::length (b->velocity) << std::endl; 

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
	
}
//----------- helper functions for update //FOR OPTIMIZATION
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

/* returns vector pulling boid $b to all hay in $fl's $affectors that are close enough */
cgra::vec2
pull_to_hay (boid *current, flock *fl){
	cgra::vec2 hay_pull = cgra::vec2 (0, 0);
	int n = 0; 
	int i;
	for (i=0; i< fl->affectors->size(); i++){ // loop for each affector // later also do if check if is hay
		cgra::vec2 hay_pos = fl->affectors->at(i).position;
		cgra::vec2 to_hay = hay_pos - current->position;
		float sheep_to_hay = cgra::length (to_hay);
		float pull_strength = 0.0;  // 0- 1 - 4
		

		if (4.0f <= sheep_to_hay && sheep_to_hay < 20.0f)
		{
			pull_strength = (sheep_to_hay - 4.0f) /8.0f; // map 4-20 -> 0-2
		} //else if (16.0f <= sheep_to_hay && sheep_to_hay < 20.0f)
		// {
		// 	pull_strength = (((sheep_to_hay -14.0f)/6)*4); // map 8-10 -> 1-4
		// }
		to_hay = cgra::normalize (to_hay);
		to_hay *= pull_strength * HAY_FACTOR; 
		if (current->state == 'f')
			to_hay /= 2; // weaker if flocking to find friends

		hay_pull += to_hay; // add to vector to return
		n++;
		//TODO DECREMENT HAY 
		//std::cout << "distance to hay: " << sheep_to_hay <<std::endl;
	}
	if (n> 0)
		hay_pull /= n;



	return hay_pull;
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
		/* closer than 2, repel strongly */
		if (cgra::distance (current->position, other->position) < COLLISION_THRESHOLD)
		{
			/*force pushing away is stronger the closer this is to other*/
			float push_scalar = COLLISION_THRESHOLD - cgra::distance (current->position, other->position);
			push_scalar *= SEPERATION_FACTOR; // scale by factor so it reasonable 
			/* vector to be returned  = this - other */
			cgra::vec2 push_vector = current->position - other->position;
			//change magnitude to be push_scalar length
			push_vector = push_vector = cgra::normalize(push_vector);
			push_vector *= push_scalar;

			seperation_force += push_vector; //add to one to be reurned
			n++;
			//std::cout << "repeling strongly" << std::endl; 
			/* closer than 8, repel weakly */
		} else if (cgra::distance (current->position, other->position) < SEPERATION_DISTANCE)
		{

			float push_scalar = f_sep_strength;
			if (current->state == 'e') // should i use flocking speration strength?
				push_scalar = e_sep_strength;
			//push vector pushes away from other
			cgra::vec2 push_vector = current->position - other->position;
			//change magnitude to be push_scalar length
			push_vector = push_vector = cgra::normalize(push_vector);
			push_vector *= push_scalar;

			seperation_force += push_vector; //add to one to be reurned
			n++;
			//std::cout << "seperating weakly " << std::endl; 
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
		if (cgra::distance (other->position, current->position) < 40){ // if within reasonabel distance
			average_position += other->position;	//add it's position to the average
			n++;
			//std::cout << "attracting to other" << std::endl; 
		}
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

	if (current->state == 'e' || current->state == 'r' )
		ret *= e_coh_strength; // weak cohesion if eating
	return ret;
}

cgra::vec2 
alignment(boid *current, flock *fl){
	//similar to cohesion, but we average velocities, and take a small portion of the perceived velocity
	//and return 
	//return cgra::vec2(0,0);

	int n=0;
	cgra::vec2 average_velocity = cgra::vec2 (0,0); 
	std::vector<boid *> flock;						// list of boids that affect this one
	construct_flock_list (&flock, fl, current, 'a');

	for (boid *other : flock)						//iterate through each boid affecting this
	{
		if (cgra::distance (other->position, current->position) < 15){ 
			//std::cout << "aligning with other" << std::endl; 
			average_velocity += other->velocity;	//add it's position to the average
			n++;
		}
	}
	if (n==0)
	{	
		return cgra::vec2(0,0); 					//if no other boids, no effect
	}
	average_velocity /= n;							//divide by num of velocities
	float factor = (ALIGNMENT_FACTOR) / 100.0f;		//how much of ave vel to return

	cgra::vec2 ret = average_velocity * factor;  	//scale
	if (current->state == 'e' || current->state == 'r' )
		ret *= eating_alignment_scalar; // if eating, dont really care about others
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