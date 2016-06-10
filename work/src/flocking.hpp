#pragma once
#include "cgra_math.hpp"
#include "geometry.hpp"
#include "terrain.hpp"

#define COHESION_FACTOR 0.05f

#define SEPERATION_DISTANCE 8.0f
#define COLLISION_THRESHOLD 2.3f
#define SEPERATION_FACTOR 1.0f

#define ALIGNMENT_FACTOR 0.4f

#define MAX_FLOCK_SPEED 0.2f
#define MAX_EAT_SPEED 0.17f

#define HAY_FACTOR 0.005f

/*	TODO when maxspeed is hit, movement looks jumpy. 
	fiddle around with other values so that we dont need this constant 
	i think, or fix
*/
/*
	0 < COHESION_FACTOR <= 100
	percent of the way to move towards the average center of flock 
	each frame
	
	0 < SEPERATION_THRESHOLD < 10?
	distance before boids start to repel each other for collisions

	0 < SEPERATION_FACTOR < 1
	used to scale down the seperation force pushing 2 boids apart


	0 < ALIGNMENT_FACTOR < 100
	percent of the average velocity to return to affect each boid's vel

	0 < HAY_FACTOR < ?
	strength at which hay pulls sheep towards it. 

	GOOD VALUES  TODO
	i like small cohesion factor & ave small thresg, perhaps magnified if there isnt another sheep close?
	seperation factor -> 1 means when they are close, they collide. 
		maybe change the implementation of sep to return weak vector if certain dist 
		and strong vec if real close?
	prolly weak alignment factor
	low ish max speed coz the're sheep
*/

struct boid
{
	Geometry *model;
	cgra::vec2 position;
	cgra::vec2 velocity;
	int id; 	// needed for equivence testing
	float rotation; 

	/* for state machine impl */
	char state = 'e'; //f=flocking e=eating p=panic r=running
	int timer=0; // ticks until can transition to next state
	int lonely = 0; // how long has been alone?  
	int bored = 630; // how bored of this patch is the sheep?
};

struct affector {// TODO make hay expire
	Geometry *model; 
	int type; // 1 = hay, 2 = wolf
	cgra::vec2 position; 
};

struct flock {
	Geometry *model; // model all members use
	int id_index;
	std::vector<boid> members; // 
	std::vector<affector> *affectors;// pointer to affector list
};

/*     FLOCK FUNCTIONS     */

/* 	initialise flock. each boid will 
	use the supplied geometry as their model when asked to render. 
*/
void init_flock (flock *, Geometry *, std::vector<affector> *);

/* 	create and add a boid to given flock.
	boid will render using the supplied geo.
	TODO: take x,z coords as arguments as position. 
 */
void add_boid (flock *, float, float, float, float);

/*
	creates new affector with specified type, geometry, and location, 
	saving it to the supplied vector
*/
void create_affector (std::vector<affector> *, Geometry *, int, float, float);

void render_affectors (std::vector<affector> *, Terrain *);

/*	updates behaviour of each boid in given flock.
	TODO: for each boid, calculate which other boids should affect it,
	construct an array containing these others and pass to update().
 */
void update_all (flock *);

/*	tell each boid to render itself.
 */
void render_all (flock *, Terrain *); 

/*     BOID FUNCTIONS     */

/*	render boid on the screen. 

 */
void render (boid *, Terrain *);

cgra::vec2 rand_vector (float);
void check_state (boid *);

/*	update specified boid's actions, influenced by each other boid
	in specified flock. 
	TODO: change argument to array of boids that should affect this,
	let flock choose whih other boids to pass in,   
	or work out which inside as seperate function.

*/
void update (boid *, flock *);

/* helper functions for update */
void construct_flock_list(std::vector<boid *> *, flock *, boid *, char );

cgra::vec2 pull_to_hay (boid *, flock *);

cgra::vec2 seperation (boid *, flock *);
cgra::vec2 cohesion (boid *, flock *);
cgra::vec2 alignment (boid *, flock *);


