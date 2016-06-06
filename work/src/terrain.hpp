#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_geometry.hpp"
#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "terrain.hpp"



struct terrain {
	float heightmap[10][10];
};

struct gridcell {
	cgra::vec3 pos[8];
	double val[8];
};

struct tri {
	cgra::vec3 pos[3];
};

void init_terrain(struct terrain *);
void generate_terrain(struct terrain *);
void get_height_at(int, int, struct terrain *, float *);
void processGridCell(struct gridcell, double, struct tri *);
vec3 vertexInterp(double, vec3, vec3, double, double);
void drawTriangle(vec3, vec3, vec3);