#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "terrain.hpp"
#include "cgra_geometry.hpp"
#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"

using namespace std;
using namespace cgra;



Terrain::Terrain(int size) {
	worldSize = size;
	stepSize = 1.0 / worldSize;
}

Terrain::~Terrain() { }

void Terrain::initialize() {
	cout << "===== Initializing Terrain =====" << endl;
}

void Terrain::generate() {
	cout << "===== Generating Terrain =====" << endl;

}

void Terrain::drawTerrain() {
	for (int x = 0; x < worldSize; x++) {
		for (int y = 0; y < worldSize; y++) {
			for (int z = 0; z < worldSize; z++) {
				processGridCell(x, y, z, stepSize);
			}
		}
	}
}

void Terrain::processGridCell(float x, float y, float z, float scale) {
	int edgeFlags;
	int cubeEdgeFlags[256];
	int connectionTable[256][16];

	float vertexValues[8];
	vec3 edgeVertices[12];

	// Grab the values from each of the cube's vertices
	for (int vSample = 0; vSample < 8; vSample++) {
		vertexValues[vSample] = sample (x + vertexOffsetVectors[vSample][0] * scale,
										y + vertexOffsetVectors[vSample][1] * scale,
										z + vertexOffsetVectors[vSample][2] * scale);
	}

	// Figure out whether the vertex is inside or outside terrain
	int index = 0;
	for (int vTest = 0; vTest < 8; vTest++) {
		if (vertexValues[vTest] <= targetValue) {
			index |= 1 << vTest; // ???
		}
	}

	cout << index << endl;

	edgeFlags = cubeEdgeFlags[index];

	// Return if the cube is entirely inside or outside the surface, don't need to do anything
	if (edgeFlags == 0) {
		return;
	}

	// Iterate through the edges and check intersections
	for (int e = 0; e < 12; e++) {
		if (edgeFlags & (1 << e)) { // ??? ("if there is an intersection on this edge")
			float offset = getOffset(vertexValues[edgeConnections[e][0]], vertexValues[edgeConnections[e][1]], targetValue);

			edgeVertices[e].x = x + (vertexOffsetVectors[edgeConnections[e][0]][0] + offset * edgeDirections[e][0]) * scale;
			edgeVertices[e].y = y + (vertexOffsetVectors[edgeConnections[e][0]][1] + offset * edgeDirections[e][1]) * scale;
			edgeVertices[e].z = z + (vertexOffsetVectors[edgeConnections[e][0]][2] + offset * edgeDirections[e][2]) * scale;

			// Calculate normals
		}
	}

	for (int t = 0; t < 5; t++) {
		if (triangleTable[index][3 * t] < 0) {
			break;
		}

		for (int c = 0; c < 3; c++) {
			int corner = triangleTable[index][3 * t + c];

			// Get colour
			// Set colour
			// Set normal
			glVertex3f(edgeVertices[corner].x, edgeVertices[corner].y, edgeVertices[corner].z);
		}
	}
}

void Terrain::setTime(float fNewTime)
{
	float tempTime = 0.0;
	int fOffset;

	for (int i = 0; i < 3; i++)
	{
		sourcePoint[i].x = 0.5;
		sourcePoint[i].y = 0.5;
		sourcePoint[i].z = 0.5;
	}

	tempTime = fNewTime;
	fOffset = 1.0 + sinf(tempTime);
	sourcePoint[0].x *= fOffset;
	sourcePoint[1].y *= fOffset;
	sourcePoint[2].z *= fOffset;
}

float Terrain::sample(float x, float y, float z) {
	double result = 0.0;
	double dx, dy, dz;

	dx = x - sourcePoint[0].x;
	dy = y - sourcePoint[0].y;
	dz = z - sourcePoint[0].z;
	result += 0.5 / (dx*dx + dy*dy + dz*dz);

	dx = x - sourcePoint[1].x;
	dy = y - sourcePoint[1].y;
	dz = z - sourcePoint[1].z;
	result += 1.0 / (dx*dx + dy*dy + dz*dz);

	dx = x - sourcePoint[2].x;
	dy = y - sourcePoint[2].y;
	dz = z - sourcePoint[2].z;
	result += 1.5 / (dx*dx + dy*dy + dz*dz);

	return result;
}

float Terrain::getOffset(float v1, float v2, float valueDesired) { // ???
	double d = v2 - v1;

	if (d == 0.0) {
		return 0.5;
	}
	return (valueDesired - v1) / d;
}
