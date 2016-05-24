#pragma once
#include "terrain.hpp"
#include <iostream>

void init_terrain(struct terrain *t) {
	std::cout << "initializin terrain" << std::endl;
}

void generate_terrain(struct terrain *t) {
	std::cout << "generating terrain" << std::endl;
}

void get_height_at(int width, int height, struct terrain *t, int *out) {
	std::cout << "requesting height " << std::endl;
}