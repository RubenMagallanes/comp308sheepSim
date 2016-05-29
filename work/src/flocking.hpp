#pragma once
#include "cgra_math.hpp"
#include "geometry.hpp"

struct boid
{
	Geometry *model;
	cgra::vec2 position;

	cgra::vec2 velocity;
	int id; 	//maybe needed for equivence testing
};

struct flock {
	int id_index;
	std::vector<boid> members; // 
};

/*     FLOCK FUNCTIONS     */

/* 	initialise flock to a certain number of boids. each boid will 
	use the supplied geometry as their model when asked to render. 
*/
void init_flock (flock *, int, Geometry *);

/* 	create and add a boid to given flock.
	boid will render using the supplied geo.
	TODO: take x,z coords as arguments as position. 
 */
void add_boid (flock *, Geometry *);

/*	updates behaviour of each boid in given flock.
	TODO: for each boid, calculate which other boids should affect it,
	construct an array containing these others and pass to update().
 */
void update_all (flock *);

/*	tell each boid to render itself.
 */
void render_all (flock *); 

/*     BOID FUNCTIONS     */

/*	render boid on the screen. 

 */
void render (boid *);

/*	update specified boid's actions, influenced by each other boid
	in specified flock. 
	TODO: change argument to array of boids that should affect this,
	let flock choose whih other boids to pass in,   
	or work out which inside as seperate function.

*/
void update (boid *, flock *);

/* helper functions for update */
cgra::vec2 seperation (boid *, flock *);
cgra::vec2 cohesion (boid *, flock *);
cgra::vec2 alignment (boid *, flock *);


