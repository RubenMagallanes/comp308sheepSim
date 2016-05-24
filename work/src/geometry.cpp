//---------------------------------------------------------------------------
//
// Copyright (c) 2016 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>


#include "cgra_math.hpp"
#include "geometry.hpp"
#include "opengl.hpp"


using namespace std;
using namespace cgra;


Geometry::Geometry(string filename, bool wireframe) {
	m_filename = filename;
	readOBJ(filename);
	if (m_triangles.size() > 0) {
		createDisplayListPoly();
		createDisplayListWire();
	}
	m_wireFrameOn = wireframe;
}


Geometry::~Geometry() { }


void Geometry::readOBJ(string filename) {

	// Make sure our geometry information is cleared
	m_points.clear();
	m_uvs.clear();
	m_normals.clear();
	m_triangles.clear();

	// Load dummy points because OBJ indexing starts at 1 not 0
	m_points.push_back(vec3(0,0,0));
	m_uvs.push_back(vec2(0,0));
	m_normals.push_back(vec3(0,0,1));


	ifstream objFile(filename);

	if(!objFile.is_open()) {
		cerr << "Error reading " << filename << endl;
		throw runtime_error("Error :: could not open file.");
	}

	cout << "Reading file " << filename << endl;
	
	// good() means that failbit, badbit and eofbit are all not set
	while(objFile.good()) {

		// Pull out line from file
		string line;
		std::getline(objFile, line);
		istringstream objLine(line);

		// Pull out mode from line
		string mode;
		objLine >> mode;

		// Reading like this means whitespace at the start of the line is fine
		// attempting to read from an empty string/line will set the failbit
		if (!objLine.fail()) {

			if (mode == "v") {
				vec3 v;
				objLine >> v.x >> v.y >> v.z;
				m_points.push_back(v);

			} else if(mode == "vn") {
				vec3 vn;
				objLine >> vn.x >> vn.y >> vn.z;
				m_normals.push_back(vn);

			} else if(mode == "vt") {
				vec2 vt;
				objLine >> vt.x >> vt.y;
				m_uvs.push_back(vt);

			} else if(mode == "f") {
				//cout << "reding face in " <<endl;
				vector<vertex> verts;
				while (objLine.good()){
					//cout << "objline is good" <<endl;
					
					vertex v;

					//-------------------------------------------------------------
					// [Assignment 1] :
					// Modify the following to parse the bunny.obj. It has no uv
					// coordinates so each vertex for each face is in the format
					// v//vn instead of the usual v/vt/vn.
					//
					// Modify the following to parse the dragon.obj. It has no
					// normals or uv coordinates so the format for each vertex is
					// v instead of v/vt/vn or v//vn.
					//
					// Hint : Check if there is more than one uv or normal in
					// the uv or normal vector and then parse appropriately.
					//-------------------------------------------------------------

					// Assignment code (assumes you have all of v/vt/vn for each vertex)
                    
					objLine >> v.p;		// Scan in position index
    				
                    if(m_normals.size() > 1) { // parse only vertex pos if no normals

                        objLine.ignore(1);	// Ignore the '/' character

                        if (m_uvs.size() > 1) // no uvs to parse

                            objLine >> v.t;		// Scan in uv (texture coord) index

                        objLine.ignore(1);	// Ignore the '/' character

                        objLine >> v.n;		// Scan in normal index
                    }

					verts.push_back(v);
					//cout << "saving vertext to array" <<endl;

				}

				// IFF we have 3 verticies, construct a triangle
				if(verts.size() >= 3){
				// modified to >= because theres a '0' at the end of each f line
					triangle tri;
					tri.v[0] = verts[0];
					tri.v[1] = verts[1];
					tri.v[2] = verts[2];
					m_triangles.push_back(tri);
					//cout << "saving triangle to array" <<endl;

				}
			}
		}
	}

	cout << "Reading OBJ file is DONE." << endl;
	cout << m_points.size()-1 << " points" << endl;
	cout << m_uvs.size()-1 << " uv coords" << endl;
	cout << m_normals.size()-1 << " normals" << endl;
	cout << m_triangles.size() << " faces" << endl;


	// If we didn't have any normals, create them
	if (m_normals.size() <= 1) createNormals();
    }


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to populate the normals for 
// the model currently loaded. Compute per face normals
// first and get that working before moving onto calculating
// per vertex normals.
//-------------------------------------------------------------
void Geometry::createNormals() {
	// YOUR CODE GOES HERE
	// per face normals
    int i, j, normal_index = 0;
    vec3 fn;    // face normal for triangle i
    vec3 edge1;
    vec3 edge2;
    for (i= 0; i< (int) m_triangles.size(); i++) {//iterate through all triangles
        
        edge1 = m_points[m_triangles[i].v[1].p] - m_points[m_triangles[i].v[0].p];
        edge2 = m_points[m_triangles[i].v[2].p] - m_points[m_triangles[i].v[0].p];
        
        fn = cross(edge1, edge2);
        
        //now add to all vertex normals
        
        for(j=0; j< 3; j++) {
            if (m_triangles[i].v[j].n == 0) { // case 1: vertex doesnt have normal
                
                //ok the problem is the fact that vertices are shared - 2 triangles
                //sharing a vertex dont share instances of a vertex strut.
                // how to figure out dupicate vectors??
                
                //this points to a spot in the m_normals vector that doesnt exist yet
                normal_index++;

                // push face normal to back, should match index now
                m_normals.push_back(fn);

                //tell vertex where it's normal is located
                m_triangles[i].v[j].n = normal_index;
                
                //whats at m_normals[m_triangles[i].v[j].n] should match fn
                /*if(m_normals[m_triangles[i].v[j].n] == fn){
                    cout << "fn matches whats at normal thing" <<endl;
                }*/ //this is good
                
            } else { // case 2: vertex already has normal
                //just add to it
                cout << "VERTEX ALREADY HAS NORMAL i=" << i << endl;
                m_normals[m_triangles[i].v[j].n] += fn;
            }
        }
        //how to tell 2 of the same vertex objs?
    }
    cout << "all normals created /added to" << endl;
   // cout << m_normals.size() << endl;
    for (i= 1; i< (int) m_normals.size(); i++){
        //m_normals[i] = normalize(m_normals[i]);// normalize vector normals
        if (length(m_normals[i]))//nan check
            m_normals[i] = normalize(m_normals[i]); // error here
        
        
        
    }
    cout << "created normals, now have " << m_normals.size()-1 << " normals" << endl;
  
}


//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as polygon model
//-------------------------------------------------------------
void Geometry::createDisplayListPoly() {
	// Delete old list if there is one
	if (m_displayListPoly) glDeleteLists(m_displayListPoly, 1);

	// Create a new list
	cout << "Creating Poly Geometry" << endl;
	m_displayListPoly = glGenLists(1);
	glNewList(m_displayListPoly, GL_COMPILE);

	// YOUR CODE GOES HERE
    
    glBegin(GL_TRIANGLES);
   
    int i;
    for (triangle t : m_triangles){ // iterate through triangles
        for (i= 0; i< 3; i++){ // iterate through verteces
            vertex v = t.v[i]; // prolly slow to save each
            vec2 uv = m_uvs[v.t];
            glTexCoord2f(uv.x, uv.y);
            vec3 norm = m_normals[v.n];
            glNormal3f(norm.x, norm.y, norm.z);
            vec3 pt = m_points[v.p];
            glVertex3f(pt.x, pt.y, pt.z);
            
            //probably faster if we do it all in one step, idk tho
            //glNormal3f(m_normals[t.v[i].n]);
            //glVertex3f(m_points[t.v[i].p]);
        }
    }
    glEnd();
    
	//MY CODE ENDS HERE
	glEndList();
	cout << "Finished creating Poly Geometry" << endl;
}
//-------------------------------------------------------------
// [Assignment 1] :
// Fill the following function to create display list
// of the obj file to show it as wireframe model
//-------------------------------------------------------------
void Geometry::createDisplayListWire() {
	// Delete old list if there is one
	if (m_displayListWire) glDeleteLists(m_displayListWire, 1);

	// Create a new list
	cout << "Creating Wire Geometry" << endl;
	m_displayListWire = glGenLists(1);
	glNewList(m_displayListWire, GL_COMPILE);

	// YOUR CODE GOES HERE
    glBegin(GL_TRIANGLES);
    
    int i;
    for (triangle t : m_triangles){ // iterate through triangles
        for (i= 0; i< 3; i++){ // iterate through verteces
            vertex v = t.v[i]; // prolly slow to save each
            vec3 norm = m_normals[v.n];
            glNormal3f(norm.x, norm.y, norm.z);
            vec3 pt = m_points[v.p];
            glVertex3f(pt.x, pt.y, pt.z);
        }
    }
    glEnd();
    
    //MY CODE ENDS HERE

	glEndList();
	cout << "Finished creating Wire Geometry" << endl;
}



void Geometry::renderGeometry() {
	if (m_wireFrameOn) {

		//-------------------------------------------------------------
		// [Assignment 1] :
		// When moving on to displaying your obj, comment out the
		// wire_cow function & uncomment the glCallList function
		//-------------------------------------------------------------

		glShadeModel(GL_SMOOTH);
		//wire_cow();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glCallList(m_displayListWire);

	} else {

		//-------------------------------------------------------------
		// [Assignment 1] :
		// When moving on to displaying your obj, comment out the
		// cow function & uncomment the glCallList function
		//-------------------------------------------------------------

		glShadeModel(GL_SMOOTH);
		//wire_cow();
       /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glCallList(m_displayListWire);*///TODO switch back yo poly not wire
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);// GL_FILL for whole polygons
        glCallList(m_displayListPoly);

	}
}


void Geometry::toggleWireFrame() {
	m_wireFrameOn = !m_wireFrameOn;
}