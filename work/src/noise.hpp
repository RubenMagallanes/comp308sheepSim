#pragma once

#include<vector>

class Noise {
private:

	// generation variables
	int width = 16;
	int height = 16;
	int resolution = 4;
	int octaves = 8; // can't change number of octaves if not iterating through generation

	// customisation variables
	float amplitude = 50.0f;
	float persistance = 0.5f;

	// private functions
	void generateWhiteNoise(std::vector<std::vector<float>> *);
	void generateSmoothNoise(std::vector<std::vector<float>>, std::vector<std::vector<float>> *, int);
	float interpolate(float, float, float);

public:

	Noise(int, int, int, int);
	~Noise();

	void generateNoise(std::vector<std::vector<float>> *);
};