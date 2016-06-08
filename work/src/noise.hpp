#pragma once

#include<vector>

class Noise {
private:

	int width = 16;
	int height = 16;
	int octaves = 8;

	void generateWhiteNoise(std::vector<std::vector<float>> *);
	void generateSmoothNoise(std::vector<std::vector<float>>, std::vector<std::vector<float>> *, int);
	float interpolate(float, float, float);

public:

	Noise(int, int, int);
	~Noise();

	void generateNoise(std::vector<std::vector<float>> *);
};