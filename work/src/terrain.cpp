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
#include "noise.hpp"

using namespace std;
using namespace cgra;

Terrain::Terrain(bool iG, int size) {
	worldSize = size;
	stepSize = 1.0 / worldSize;
	isGround = iG;

	n = new Noise(worldSize, worldSize, noiseResolution, 8);
	noise.resize(worldSize * noiseResolution, vector<float>(worldSize * noiseResolution));
}

Terrain::~Terrain() { }

void Terrain::initialize() {
	cout << "===== Initializing Terrain =====" << endl;
}

void Terrain::generate() {
	cout << "===== Generating Terrain =====" << endl;
	
	n->generateNoise(5, &noise, &min, &max);

	cout << "Generated " << noise.size() << " points of noise" << endl;
	
	if (m_displayList) glDeleteLists(m_displayList, 1);
	cout << "===== Creating Terrain Display List =====" << endl;
	
	m_displayList = glGenLists(1);
	glNewList(m_displayList, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	
	// iterate through noise grid in quads, drawing two triangles
	for (int x = 0; x < worldSize * noiseResolution - 1; x++) {
		for (int z = 0; z < worldSize * noiseResolution - 1; z++) {
		  
			// +----------+ //
			// |v1      v2| //
			// |     /    | // Splitting into
			// |    /     | // two triangles
			// |v4      v3| //
			// +----------+ //

			// calculate positions
			vec3 v1p = { x/noiseResolution, noise[x][z] * (noise[x][z]*3), z/noiseResolution };
			vec3 v2p = { x/noiseResolution, noise[x][z+1] * (noise[x][z+1]*3), (z+1)/noiseResolution };
			vec3 v3p = { (x+1)/noiseResolution, noise[x+1][z+1] * (noise[x+1][z+1]*3), (z+1)/noiseResolution };
			vec3 v4p = { (x+1)/noiseResolution, noise[x+1][z] * (noise[x+1][z]*3), z/noiseResolution };
			
			// calculate normals for each triangle based on v1 and v4
			vec3 v1n = normalize(cross((v2p - v1p), (v4p - v1p)));
			vec3 v2n = normalize(cross((v3p - v2p), (v1p - v2p)));
			vec3 v3n = normalize(cross((v4p - v3p), (v2p - v3p)));
			vec3 v4n = normalize(cross((v1p - v4p), (v3p - v4p)));
			
			// average normals
			vec3 t1n = (v1n + v2n + v4n) / 3.0f;
			vec3 t2n = (v2n + v3n + v4n) / 3.0f;
			
			// calculate colour for triangle 1
			vec3 t1c = { 0.0f, 1 - (((noise[x][z] + noise[x][z + 1] + noise[x + 1][z]) / 3.0f) - min) / max + 0.1f, 0.2f };
			vec3 t2c = { 0.0f, 1 - (((noise[x+1][z+1] + noise[x][z + 1] + noise[x + 1][z]) / 3.0f) - min) / max + 0.1f, 0.2f };

			// triangle 1
			glVertex3f(v1p.x, v1p.y, v1p.z); // v1
			glNormal3f(t1n.x, t1n.y, t1n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
			
			glVertex3f(v2p.x, v2p.y, v2p.z); // v2
			glNormal3f(t1n.x, t1n.y, t1n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
			
			glVertex3f(v4p.x, v4p.y, v4p.z); // v4
			glNormal3f(t1n.x, t1n.y, t1n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
			
			// triangle 2
			glVertex3f(v3p.x, v3p.y, v3p.z); // v3
			glNormal3f(t2n.x, t2n.y, t2n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
			
			glVertex3f(v2p.x, v2p.y, v2p.z); // v2
			glNormal3f(t2n.x, t2n.y, t2n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
			
			glVertex3f(v4p.x, v4p.y, v4p.z); // v4
			glNormal3f(t2n.x, t2n.y, t2n.z);
			glColor3f(t1c.x, t1c.y, t1c.z);
		}
	}
	
	glEnd();
	glEndList();
	
	cout << "===== Display List Created =====" << endl;
}

void Terrain::drawTerrain() {
	// Marching Cubes
	/*for (int x = 0; x < worldSize; x++) {
		for (int y = 0; y < worldSize; y++) {
			for (int z = 0; z < worldSize; z++) {
				processGridCell(x * stepSize, y * stepSize, z * stepSize, stepSize);
			}
		}
	}*/

	//glPushMatrix();
	//glTranslatef(-(worldSize / 2.0) + 0.5f, 0.0f, -(worldSize / 2.0) + 0.5f);

	// Polygons
	/*for (int x = 0; x < worldSize * noiseResolution; x+=2) {
		for (int z = 0; z < worldSize * noiseResolution; z++) {
			//glPushMatrix();

			//glTranslatef(x/noiseResolution, noise[x][z], z/noiseResolution);
			//cgraSphere(0.1);

			//glPopMatrix();
		}
	}*/
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glCallList(m_displayList);

	//glPopMatrix();
}

void Terrain::processGridCell(float x, float y, float z, float scale) {
	int edgeFlags;
	int cubeEdgeFlags[256];
	int connectionTable[256][16];

	float vertexValues[8];
	vec3 edgeVertices[12];

	// Grab the values from each of the cube's vertices
	if (y >= 0.5) {
		for (int vSample = 0; vSample < 8; vSample++) {
			/*vertexValues[vSample] = sample (x + vertexOffsetVectors[vSample][0] * scale,
			y + vertexOffsetVectors[vSample][1] * scale,
			z + vertexOffsetVectors[vSample][2] * scale);*/

			vertexValues[vSample] = -1.0;
			cout << "Above ground" << endl;
		}
	}
	else {
		for (int vSample = 0; vSample < 8; vSample++) {
			/*vertexValues[vSample] = sample (x + vertexOffsetVectors[vSample][0] * scale,
			y + vertexOffsetVectors[vSample][1] * scale,
			z + vertexOffsetVectors[vSample][2] * scale);*/

			vertexValues[vSample] = 1.0;
			cout << "============================ Below ground" << endl;
		}
	}

	// Figure out whether the vertex is inside or outside terrain
	int index = 0;
	for (int vTest = 0; vTest < 8; vTest++) {
		if (vertexValues[vTest] <= targetValue) {
			index |= 1 << vTest;
		}
	}

	//cout << index << endl;

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
			cout << "Drawing a triangle" << endl;
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
	int fOffset;

	for (int i = 0; i < 3; i++)
	{
		sourcePoint[i].x = 0.5;
		sourcePoint[i].y = 0.5;
		sourcePoint[i].z = 0.5;
	}

	sampleTime = fNewTime;
	fOffset = 1.0 + sinf(sampleTime);
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

float Terrain::getHeightAt(float x, float y) {
	return noise[x][y];
}