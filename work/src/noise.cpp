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

void Noise::generateNoise(float numOct, vector<vector<float>> *finalNoise, float *min, float *max) {
	float resWidth = width * resolution;
	float resHeight = height * resolution;

	*min = 999.9;
	*max = -999.9;

	vector<vector<float>> whiteNoise (resWidth, vector<float>(resHeight));
	generateWhiteNoise(&whiteNoise);

	// Doing this manually because I suck at pointers
	vector<vector<float>> smoothNoise1(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise2(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise3(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise4(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise5(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise6(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise7(resWidth, vector<float>(resHeight));
	vector<vector<float>> smoothNoise8(resWidth, vector<float>(resHeight));
	vector<vector<vector<float>>> allNoise (6, vector<vector<float>>(resWidth, vector<float>(resHeight, NULL)));
	
	generateSmoothNoise(whiteNoise, &smoothNoise1, 0);
	allNoise[0] = smoothNoise1;
	
	generateSmoothNoise(whiteNoise, &smoothNoise2, 1);
	allNoise[1] = smoothNoise2;
	
	generateSmoothNoise(whiteNoise, &smoothNoise3, 2);
	allNoise[2] = smoothNoise3;
	
	generateSmoothNoise(whiteNoise, &smoothNoise4, 3);
	allNoise[3] = smoothNoise4;
	
	generateSmoothNoise(whiteNoise, &smoothNoise5, 4);
	allNoise[4] = smoothNoise5;
	
	generateSmoothNoise(whiteNoise, &smoothNoise6, 5);
	allNoise[5] = smoothNoise6;

	float totalAmplitude = 0.0f;

	// blending
	for (int oct = numOct-1; oct >= 0; oct--) {
		amplitude *= persistance;
		totalAmplitude += amplitude;

		// add all octaves together
		for (int x = 0; x < resWidth; x++) {
			for (int y = 0; y < resHeight; y++) {
				(*finalNoise)[x][y] += allNoise[oct][x][y] * amplitude;
				//cout << allNoise[oct][x][y] * amplitude << endl;
			}
		}
	}

	// average amplitudes and set min/max
	for (int x = 0; x < resWidth; x++) {
		for (int y = 0; y < resHeight; y++) {
			(*finalNoise)[x][y] /= totalAmplitude;

			if ((*finalNoise)[x][y] > *max)
				*max = (*finalNoise)[x][y];

			if ((*finalNoise)[x][y] < *min)
				*min = (*finalNoise)[x][y];
		}
	}
}

void Noise::generateWhiteNoise(vector<vector<float>> *noise) {
	srand(static_cast <unsigned> (time(0))); // seed rand

	for (int x = 0; x < (*noise).size(); x++) {
		for (int y = 0; y < (*noise)[0].size(); y++) {
			// generate random float between 0 - 1
			(*noise)[x][y] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
	}
}

void Noise::generateSmoothNoise(vector<vector<float>> baseNoise, vector<vector<float>> *smoothNoise, int octave) {
	int samplePeriod = pow(2, octave); // 2^octave
	float sampleFrequency = 1.0f / samplePeriod; // f = 1/T

	for (int x = 0; x < (*smoothNoise).size(); x++) {
		// sample horizontal points
		int samplex0 = (x / samplePeriod) * samplePeriod;
		int samplex1 = (samplex0 + samplePeriod) % (*smoothNoise).size();
		float horizBlend = (x - samplex0) * sampleFrequency;

		for (int y = 0; y < (*smoothNoise)[0].size(); y++) {
			// sample vertical points
			int sampley0 = (y / samplePeriod) * samplePeriod;
			int sampley1 = (sampley0 + samplePeriod) % (*smoothNoise)[0].size();
			float vertBlend = (y - sampley0) * sampleFrequency;

			float top = interpolate(	baseNoise[samplex0][sampley0],
							baseNoise[samplex1][sampley0], horizBlend);

			float bottom = interpolate(	baseNoise[samplex0][sampley1],
							baseNoise[samplex1][sampley1], horizBlend);

			(*smoothNoise)[x][y] = interpolate(top, bottom, vertBlend);
		}
	}
}

// interpolate along the edges depending on the weightings - can be used for FBM also
float Noise::interpolate(float v1, float v2, float d) {
	return v1 * (1 - d) + d * v2;
}