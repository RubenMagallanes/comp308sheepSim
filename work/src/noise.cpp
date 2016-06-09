#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <ctime>

#include "noise.hpp"

using namespace std;

Noise::Noise(int w, int h, int r, int o) {
	width = w;
	height = h;
	resolution = r;
	octaves = o;
}

Noise::~Noise() { }

void Noise::generateNoise(vector<vector<float>> *perlinNoise) {
	float resWidth = width * resolution;
	float resHeight = height * resolution;

	vector<vector<float>> whiteNoise (width, vector<float>(height));
	generateWhiteNoise(&whiteNoise);

	// Doing this manually because I suck at pointers
	vector<vector<float>> smoothNoise1(width, vector<float>(height));
	vector<vector<float>> smoothNoise2(width, vector<float>(height));
	vector<vector<float>> smoothNoise3(width, vector<float>(height));
	vector<vector<float>> smoothNoise4(width, vector<float>(height));
	vector<vector<float>> smoothNoise5(width, vector<float>(height));
	vector<vector<float>> smoothNoise6(width, vector<float>(height));
	vector<vector<float>> smoothNoise7(width, vector<float>(height));
	vector<vector<float>> smoothNoise8(width, vector<float>(height));
	vector<vector<vector<float>>> allNoise (octaves, vector<vector<float>>(width, vector<float>(height, NULL)));

	//for (int i = 0; i < octaves; i++) {
		generateSmoothNoise(whiteNoise, &smoothNoise1, 0);
		allNoise[0] = smoothNoise1;
		generateSmoothNoise(whiteNoise, &smoothNoise2, 0);
		allNoise[1] = smoothNoise2;
		generateSmoothNoise(whiteNoise, &smoothNoise3, 0);
		allNoise[2] = smoothNoise3;
		generateSmoothNoise(whiteNoise, &smoothNoise4, 0);
		allNoise[3] = smoothNoise4;
		generateSmoothNoise(whiteNoise, &smoothNoise5, 0);
		allNoise[4] = smoothNoise5;
		generateSmoothNoise(whiteNoise, &smoothNoise6, 0);
		allNoise[5] = smoothNoise6;
		generateSmoothNoise(whiteNoise, &smoothNoise7, 0);
		allNoise[6] = smoothNoise7;
		generateSmoothNoise(whiteNoise, &smoothNoise8, 0);
		allNoise[7] = smoothNoise8;
	//}

	float totalAmplitude = 0.0f;

	// blending
	for (int o = octaves - 1; o > 0; o--) {
		amplitude *= persistance;
		totalAmplitude += amplitude;

		// add all octaves together
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				(*perlinNoise)[x][y] += allNoise[o][x][y] * amplitude;
			}
		}
	}

	// average amplitudes
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			(*perlinNoise)[x][y] /= totalAmplitude;
		}
	}
}

void Noise::generateWhiteNoise(vector<vector<float>> *noise) {
	srand(static_cast <unsigned> (time(0)));

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			(*noise)[x][y] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
	}
}

void Noise::generateSmoothNoise(vector<vector<float>> baseNoise, vector<vector<float>> *smoothNoise, int octave) {
	int samplePeriod = 1 << octave;
	float sampleFrequency = 1.0f / samplePeriod;

	for (int i = 0; i < width; i++) {
		// horizontal sampling indices
		int samplei0 = (i / samplePeriod) * samplePeriod;
		int samplei1 = (samplei0 + samplePeriod) % width;
		float horizBlend = (i - samplei0) * sampleFrequency;

		for (int j = 0; j < height; j++) {
			// vertical sampling indices
			int samplej0 = (j / samplePeriod) * samplePeriod;
			int samplej1 = (samplej0 + samplePeriod) % height;
			float vertBlend = (j - samplej0) * sampleFrequency;

			float top = interpolate(baseNoise[samplei0][samplej0],
									baseNoise[samplei1][samplej0], horizBlend);

			float bottom = interpolate(	baseNoise[samplei0][samplej1],
										baseNoise[samplei1][samplej1], horizBlend);

			(*smoothNoise)[i][j] = interpolate(top, bottom, vertBlend);
		}
	}
}

float Noise::interpolate(float v1, float v2, float d) {
	return v1 * (1 - d) + d * v2;
}