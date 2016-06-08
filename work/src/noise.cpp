#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include "noise.hpp"

using namespace std;

Noise::Noise(int w, int h, int o) {
	width = w;
	height = h;
	octaves = o;
}

Noise::~Noise() { }

void Noise::generateNoise(vector<vector<float>> *perlinNoise) {
	vector<vector<float>> whiteNoise (width, vector<float>(height));
	generateWhiteNoise(&whiteNoise);

	vector<vector<float>> smoothNoise (width, vector<float>(height));
	vector<vector<vector<float>>> allNoise (octaves, vector<vector<float>>(width, vector<float>(height)));

	float persistance = 0.5f;

	for (int i = 0; i < octaves; i++) {
		generateSmoothNoise(whiteNoise, &smoothNoise, i);
		allNoise[i] = smoothNoise;
	}

	float amplitude = 1.0f;
	float totalAmplitude = 0.0f;

	// blending
	for (int o = octaves - 1; o > 0; o--) {
		amplitude *= persistance;
		totalAmplitude += amplitude;

		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				(*perlinNoise)[x][y] += allNoise[o][x][y] * amplitude;
			}
		}
	}

	// normalisation
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			(*perlinNoise)[x][y] /= totalAmplitude;
		}
	}
}

void Noise::generateWhiteNoise(vector<vector<float>> *noise) {

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			(*noise)[x][y] = rand();
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
									baseNoise[samplej0][samplej0], horizBlend);

			float bottom = interpolate(	baseNoise[samplei0][samplej1],
										baseNoise[samplej1][samplej1], horizBlend);

			(*smoothNoise)[i][j] = interpolate(top, bottom, vertBlend);
		}
	}
}

float Noise::interpolate(float v1, float v2, float d) {
	return v1 * (1 - d) + (d * v2);
}