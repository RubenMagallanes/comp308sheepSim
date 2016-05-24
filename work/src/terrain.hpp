#pragma once

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

struct terrain {
	float heightmap[MAP_WIDTH][MAP_HEIGHT];
};

void init_terrain(struct terrain *);
void generate_terrain(struct terrain *);
void get_height_at(int, int, struct terrain *, float*);