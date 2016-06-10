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
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>

#include "cgra_geometry.hpp"
#include "cgra_math.hpp"
#include "simple_image.hpp"
#include "simple_shader.hpp"
#include "opengl.hpp"
#include "geometry.hpp"
#include "terrain.hpp"
#include "flocking.hpp"

using namespace std;
using namespace cgra;

// Window
//
GLFWwindow* g_window;

// Terrain
//
Terrain *tMain = nullptr;
Terrain *tGround = nullptr;

// Projection values
// 
float g_fovy = 20.0;
float g_znear = 0.1;
float g_zfar = 100000.0;


// Mouse controlled Camera values
//
bool g_leftMouseDown = false;
vec2 g_mousePosition;

float g_pan_x = 0;
float g_pan_z = 0;

float g_pitch = 20.0f;
float g_yaw = 45.0f;
float g_zoom = 1.0f;


//translation across the land
//x and y of this vector are actually x, z of our land
vec2 translate_map = vec2(0,0); 


float keySensitivity = 0.5;

// Values and fields to showcase the use of shaders
// Remove when modifying main.cpp for Assignment 3
//
bool g_useShader = false;
GLuint g_texture0 = 0;

GLuint g_shader = 0;

//GLuint my_texture = 1;

Geometry *geo_sphere = nullptr;
Geometry *geo_table = nullptr;
//obj file for sheep. 
//should only really need one ref to this, have each boid store
//it's own transform and rotation vectors. 
Geometry *geo_sheep = nullptr;
Geometry *geo_hay = nullptr;

flock herd;
std::vector<affector> hay_set; 

int terrainSize = 128;


GLfloat light_position1[] = { 0.0,1.0,1.0 ,0.0}; // direc light dir


GLfloat point_1_pos[] = { 10.0, 5.0, -10.0, 1.0 }; // point light 1 location
GLfloat point_2_pos[] = { 10.0, 5.0, 10.0, 1.0 }; // point light 1 location
GLfloat point_3_pos[] = { -10.0, 5.0, -10.0, 1.0 }; // point light 1 location
GLfloat point_4_pos[] = { -10.0, 5.0, 10.0, 1.0 }; // point light 1 location


//w,a,s,d = camera movement (translation) q,e = rotation, f,c = pitch up/down
int w_down=0, a_down=0, s_down=0, d_down=0, 
	q_down=0, e_down=0, 
	f_down=0, c_down=0,
	z_down=0, x_down=0,
	p_down=0; 

float sensitivity = 0.5; // how sensitive the mouse is 

// Mouse Button callback
// Called for mouse movement event on since the last glfwPollEvents
//
void cursorPosCallback(GLFWwindow* win, double xpos, double ypos) {
	// cout << "Mouse Movement Callback :: xpos=" << xpos << "ypos=" << ypos << endl;
	if (g_leftMouseDown) {
		g_yaw -= (g_mousePosition.x - xpos) * sensitivity;
		g_pitch -= (g_mousePosition.y - ypos) * sensitivity;
	}
	g_mousePosition = vec2(xpos, ypos);
}


// Mouse Button callback
// Called for mouse button event on since the last glfwPollEvents
//
void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {

}

/*
	resets the screent to the center of the scene. 
*/
void resetScreen(){

//TODO
}

// Scroll callback
// Called for scroll event on since the last glfwPollEvents
//
void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
	// cout << "Scroll Callback :: xoffset=" << xoffset << "yoffset=" << yoffset << endl;
	g_zoom -= yoffset * g_zoom * 0.2; // UNCOMMENT TO ENABLE MOUSE SCROLL ZOOMING
}


// Keyboard callback
// Called for every key event on since the last glfwPollEvents
//
void keyCallback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	//cout << "Key Callback :: key=" << key << " scancode=" << scancode
	 	//<< " action=" << action << " mods=" << mods << endl;


	 	switch (char(key))
	 	{
	 		case 'W': 
				w_down = action;
	 		break;
	 		case 'A':
				a_down = action;
	 		break;
	 		case 'S':
				s_down = action;
	 		break;
	 		case 'D':
				d_down = action;
	 		break;
	 		case 'Q':
				q_down = action;
	 		break;
	 		case 'E':
				e_down = action;
	 		break;
	 		case 'F':
				f_down = action;
	 		break;
	 		case 'C':
				c_down = action;
	 		break;
	 		case 'Z':
	 			z_down = action;
	 		break;
	 		case 'X':
	 			x_down = action;
	 		break;
	 		case 'P':
	 			p_down = action;
	 		break;
	 		case ' ':
	 			if (action)
	 				resetScreen();
	 		break;
	 		//default:
	 		//do nothing
	 	}
}


// Character callback
// Called for every character input event on since the last glfwPollEvents
//
void charCallback(GLFWwindow *win, unsigned int c) {
	// cout << "Char Callback :: c=" << char(c) << endl;
	// Not needed for this assignment, but useful to have later on
}


// Sets up where and what the light is
// Called once on start up
// 
void initLight() {

	cout << "init light" << endl;

    glClearColor (0.0, 0.0, 0.0, 0.0);// 0,0,0,0 is initial 
    glShadeModel (GL_SMOOTH); // smooth shading not flat

    // AMBIENT LIGHT SOURCE
    
    GLfloat light_position0[] = { 1.0, 1.0, 1.0, 0.0 };// 1.0, 1.0, 1.0, 0.0 }; 
	float ambient_color0[] = {0.2,0.2,0.2,1.0};//{0,0,0,1} init value
    float diffuse_color0[] = {0,0,0,0};//{1,1,1,1};
    float specular_color0[] = {0,0,0,0};//{1,1,1,1}

    glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_color0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_color0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_color0); 
   glEnable(GL_LIGHT0); //ENABLE AMBIENT LIGHT

    // DIRECTIONAL LIGHT SOURCE
    float diffuse_color1[] = {0.3,0.3,0.3,1.0};//{1,1,1,1};
    float specular_color1[] = {1.0,1.0,1.0,1.0};//{1,1,1,1} // odnt know we need this for directional


    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_color1);
   glEnable(GL_LIGHT1);


    //SPOTLIGHT

     // pointing down
    
/*
    float diffuse_color2[] = {1.0,1.0,1.0,1.0};//{1,1,1,1};
   // float specular_color2[] = {1.0,1.0,1.0,1.0};//{1,1,1,1} 
  

    glLightfv(GL_LIGHT2, GL_POSITION, light_position2); // position as spot light
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2); // point
    //glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,10.0f); // how focused is spotlight
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f); // angle

    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_color2);
    //glLightfv(GL_LIGHT2, GL_SPECULAR, specular_color2);
    glEnable(GL_LIGHT2);//disable spotlight

*/
	//-----fields------------------
	
	//-----local-----------
/*
	float point_diffuse[] = {0.2, 0.2, 0.2, 1.0};
	float point_specular[] = {0.3, 0.3, 0.3, 1.0};



	glLightfv(GL_LIGHT3, GL_POSITION, point_1_pos); // position point light
	glLightfv(GL_LIGHT3, GL_DIFFUSE, point_diffuse); // set diffuse color
    glLightfv(GL_LIGHT3, GL_SPECULAR, point_specular); // set specular color
    glEnable(GL_LIGHT3);//right

    glLightfv(GL_LIGHT4, GL_POSITION, point_2_pos); // position point light
	glLightfv(GL_LIGHT4, GL_DIFFUSE, point_diffuse); // set diffuse color
    glLightfv(GL_LIGHT4, GL_SPECULAR, point_specular); // set specular color
    glEnable(GL_LIGHT4); // close

    glLightfv(GL_LIGHT5, GL_POSITION, point_3_pos); // position point light
	glLightfv(GL_LIGHT5, GL_DIFFUSE, point_diffuse); // set diffuse color
    glLightfv(GL_LIGHT5, GL_SPECULAR, point_specular); // set specular color
    glEnable(GL_LIGHT5); // back

    glLightfv(GL_LIGHT6, GL_POSITION, point_4_pos); // position point light
	glLightfv(GL_LIGHT6, GL_DIFFUSE, point_diffuse); // set diffuse color
    glLightfv(GL_LIGHT6, GL_SPECULAR, point_specular); // set specular color
    glEnable(GL_LIGHT6); // left




	//now we can use glcolor to set material color i think
	//these make eerything flat again
	//glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
	glEnable(GL_LIGHTING);
	*/

	glEnable(GL_DEPTH_TEST);
    glEnable ( GL_COLOR_MATERIAL ) ;


}


// An example of how to load a texure from a hardcoded location
//

void initTexture() { //save incase i ruin the one above
	/*//Image tex("./work/res/textures/wood.jpg");
	// Use slot 0, need to use GL_TEXTURE1 ... etc if using more than one texture PER OBJECT
	glActiveTexture(GL_TEXTURE0); 
	glGenTextures(1, &g_texture0); // Generate texture ID
	glBindTexture(GL_TEXTURE_2D, g_texture0); // Bind it as a 2D texture

	// Setup sampling strategies
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Finnaly, actually fill the data into our texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, tex.w, tex.h, tex.glFormat(), GL_UNSIGNED_BYTE, tex.dataPointer()); */
}

// An example of how to load a shader from a hardcoded location
//
void initShader() {

}


// Sets up where the camera is in the scene
// 
void setupCamera(int width, int height) {
	// Set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(g_fovy, width / float(height), g_znear, g_zfar);

	// Set up the view part of the model view matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// //translate around area.
	// if (w_down)
	// 	translate_map.y -= 0.3;

	// glTranslatef(translate_map.x, translate_map.y, 0.0);
	//rotate, translatem then tilt i think. zoom shouldnt matter

	//zoom, rotate and pitch. 
	if (z_down)
		g_zoom += 0.1 * keySensitivity;
	if (x_down)
		g_zoom -= 0.1 * keySensitivity;
	glTranslatef(-0, -1, -50 * g_zoom);
	
	//pitch from keys
	if (f_down)
		g_pitch -= keySensitivity;
	if (c_down)
		g_pitch += keySensitivity;

	glRotatef(g_pitch, 1, 0, 0);
	
	//pan forward/backward from keys
	if (w_down)
		g_pan_z += g_zoom;
	if (s_down)
		g_pan_z -= g_zoom;
	  
	//pan left/right from keys
	if (a_down)
		g_pan_x += g_zoom;
	if (d_down)
		g_pan_x -= g_zoom;
	
	//translate around area.
	glTranslatef(g_pan_x, 0, g_pan_z);
	
	//rotation from keys
	if (q_down)
		g_yaw += keySensitivity;
	if (e_down)
		g_yaw -= keySensitivity;
	
	glRotatef(g_yaw, 0, 1, 0);
	
}

void initGeometry(){
    //load in all objs, save to geometrys
    /*
    geo_box    = new Geometry("./work/res/assets/box.obj");
    geo_bunny  = new Geometry("./work/res/assets/bunny.obj");
    */
    geo_sphere = new Geometry("./work/res/assets/sphere.obj", false);
    /*
    geo_teapot = new Geometry("./work/res/assets/teapot.obj");
    geo_torus  = new Geometry("./work/res/assets/torus.obj");
*/
    geo_table  = new Geometry("./work/res/assets/table.obj", false);

    geo_sheep  = new Geometry("./work/res/assets/sheep.obj", false);
    geo_hay	   = new Geometry("./work/res/assets/hay.obj", false);
}


void temp_terrain (){
	glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_COLOR_MATERIAL);


    
    


	GLfloat mat_specularW [] = { 0.5, 0.5, 0.5, 1.0 };  
	GLfloat mat_shininessW[] = { 120.0 };  

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularW);  
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessW);
	GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    glPushMatrix();
    glTranslatef(0.0f, -0.4f, 0.0f);
    //glScalef(2.0, 0.0, 2.0); // make table huge
    geo_table->renderGeometry();
    //draw more tables for reference
    glTranslatef(20.0, 0.0, 20.0);
	geo_table->renderGeometry();
	glTranslatef(0.0, 0.0, -40.0);
	geo_table->renderGeometry();
	glTranslatef(-40.0, 0.0, 0.0);
	geo_table->renderGeometry();
	glTranslatef(0.0, 0.0, 40.0);
	geo_table->renderGeometry();

    glPopMatrix();
    glPushMatrix();
    glTranslatef(10.0, 5.0, -10.0);
    cgraSphere(0.5); //back right corner
   	glTranslatef(0.0, 0.0, 20);
   	cgraSphere(0.5); //front right corner
   	glTranslatef(-20.0, 0.0, 0.0);
   	cgraSphere(0.5);
   	glTranslatef(0.0, 0.0, -20.0);
   	cgraSphere(0.5);
    glPopMatrix();

}
void set_sheep_color (){
	//colors for blue sheep material	
	GLfloat mat_specularW2 [] = { 0.2, 0.2, 0.2, 1.0 };  
	GLfloat mat_shininessW2[] = { 100.0 };  

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularW2);  
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessW2);
	GLfloat white2[] = {0.9f, 0.9f, 0.9f, 1.f};
	GLfloat blue[] = {0.7, 0.7, 0.7, 1.f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
}
void set_hay_color (){
	//colors for blue sheep material	
	GLfloat mat_specularW2 [] = { 0.2, 0.2, 0.2, 1.0 };  
	GLfloat mat_shininessW2[] = { 128.0 };//not shiny  

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularW2);  
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessW2);
	GLfloat white2[] = {0.9f, 0.9f, 0.9f, 1.f};
	GLfloat yello[] = {0.6, 0.6, 0.2, 1.f};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yello);
}

GLenum tPolygonMode = GL_FILL;

void initTerrain() {
	tMain = new Terrain(false, terrainSize);
	tMain->generate();

	tGround = new Terrain(true, terrainSize);
	tGround->generate();
}

// Draw function

//for time based updating
double lastTime = glfwGetTime();

int nbFrames = 0;
 
void render(int width, int height) {

     // Measure speed
    double currentTime = glfwGetTime();
    nbFrames++;
    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%.4f ms/frame, ", 1000.0/double(nbFrames));
        printf("%.1f fps\n", 1/(1/double(nbFrames)));
        nbFrames = 0;
        lastTime += 1.0;
    }

    // main render loop

/*
fps counter
*/
 //init_terrain(&t);



     // Measure speed
   
     ///main render loop




	// Sky blue background
	glClearColor(0.53f, 0.81f, 0.98f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Enable flags for normal rendering
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	setupCamera(width, height);


	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);// directional light

    // // DISABLE FOR COLORS SEt BY SET HAY COLOR
    glEnable(GL_COLOR_MATERIAL);

    // draw main terrain
    glPushMatrix();
	glScalef(100.0f, 100.0f, 100.0f);
	float centerHeightMain = tMain->getHeightAt(0.0f, 0.0f);
    glTranslatef(-((float)terrainSize/2), -centerHeightMain * 8.0 - 1.0, -((float)terrainSize/2));
   // tMain->drawTerrain();
    //geo_sphere->renderGeometry();
    glPopMatrix();

	// draw ground terrain
	glPushMatrix();
	glScalef(100.0f, 100.0f, 100.0f);
	float centerHeightGround = tGround->getHeightAt(0.0f, 0.0f);
	glTranslatef(-((float)terrainSize / 2), -centerHeightGround * 2.0, -((float)terrainSize / 2));
	//tGround->drawTerrain();
	//geo_sphere->renderGeometry();
	glPopMatrix();

 //    //GOLD SPHERE
 //    GLfloat mat_ambientG  [] = {0.25, 0.2, 0.07, 1};
 //    GLfloat mat_specularG [] = { 1.0,1.0,1.0 ,1};  
	// GLfloat mat_shininessG[] = { 5 };  

	// glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambientG);
	// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularG);  
	// glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessG);
	// GLfloat gold[] = {0.75, 0.61, 0.23, 1.f};
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, gold);

 //    glPushMatrix();
 //    glTranslatef(0.0,1.5,0.0);
 //    //geo_sphere->renderGeometry();
 //    glPopMatrix();

	// GLfloat mat_specularW [] = { 1.0, 1.0, 1.0, 1.0 };
	// GLfloat mat_shininessW[] = { 50.0 };  

	// glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularW);  
	// glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininessW);
	// GLfloat white[] = {0.8f, 0.8f, 0.8f, 1.f};
	// glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
 //    glPushMatrix();
 //    glTranslatef(0.0f, -0.4f, 0.0f);
 //    // geo_table->renderGeometry();//TODO uncomment
 //    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);

	//glLightfv(GL_LIGHT3, GL_POSITION, light_position3); // 

	//glLightfv(GL_LIGHT2, GL_POSITION, light_position2); // spot
	//glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction2); // point
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);// directional light 
	//glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, angle); // angle
	// Without shaders
/*
	glLightfv(GL_LIGHT3, GL_POSITION, point_1_pos);
	glLightfv(GL_LIGHT4, GL_POSITION, point_2_pos);
	glLightfv(GL_LIGHT5, GL_POSITION, point_3_pos);
	glLightfv(GL_LIGHT6, GL_POSITION, point_4_pos);*/

    //temp_terrain(); // render my temp terrain enviro
    //glDisable(GL_TEXTURE_2D);

    //RENDER FLOCK AND SHIT
      currentTime = glfwGetTime();
    
    /* time based updating - updates at max every 0.01 sec  */
    if ( currentTime - lastTime >= 0.017f  && p_down){ // 1/ 0.017 = 60 updates per sec (58)
        // reset timer 
        update_all(&herd); // update all boids position and intention
        lastTime += 0.017f;
    }
    glDisable(GL_COLOR_MATERIAL);

    cgra::cgraSphere (0.5);

	set_hay_color();
	render_affectors(&hay_set);
	
    set_sheep_color();
    render_all(&herd);

	
	//cgra::cgraSphere (0.5);

	glEnable(GL_COLOR_MATERIAL);

	// Disable flags for cleanup (optional)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);
}


// Forward decleration for cleanliness (Ignore)
void APIENTRY debugCallbackARB(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, GLvoid*);


//Main program
// 
int main(int argc, char **argv) {

	// Initialize the GLFW library
	if (!glfwInit()) {
		cerr << "Error: Could not initialize GLFW" << endl;
		abort(); // Unrecoverable error
	}

	// Get the version for GLFW for later
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);

	// Create a windowed mode window and its OpenGL context
	g_window = glfwCreateWindow(880, 660, "Hello World", nullptr, nullptr);
	//640, 480
	if (!g_window) {
		cerr << "Error: Could not create GLFW window" << endl;
		abort(); // Unrecoverable error
	}

	// Make the g_window's context is current.
	// If we have multiple windows we will need to switch contexts
	glfwMakeContextCurrent(g_window);



	// Initialize GLEW
	// must be done after making a GL context current (glfwMakeContextCurrent in this case)
	glewExperimental = GL_TRUE; // required for full GLEW functionality for OpenGL 3.0+
	GLenum err = glewInit();
	if (GLEW_OK != err) { // Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
		abort(); // Unrecoverable error
	}



	// Print out our OpenGL verisions
	cout << "Using OpenGL " << glGetString(GL_VERSION) << endl;
	cout << "Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "Using GLFW " << glfwMajor << "." << glfwMinor << "." << glfwRevision << endl;



	// Attach input callbacks to g_window
	glfwSetCursorPosCallback(g_window, cursorPosCallback);
	glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
	glfwSetScrollCallback(g_window, scrollCallback);
	glfwSetKeyCallback(g_window, keyCallback);
	glfwSetCharCallback(g_window, charCallback);



	// Enable GL_ARB_debug_output if available. Not nessesary, just helpful
	if (glfwExtensionSupported("GL_ARB_debug_output")) {
		// This allows the error location to be determined from a stacktrace
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		// Set the up callback
		glDebugMessageCallbackARB(debugCallbackARB, nullptr);
		glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
		cout << "GL_ARB_debug_output callback installed" << endl;
	}
	else {
		cout << "GL_ARB_debug_output not available. No worries." << endl;
	}

    initGeometry();
	initTerrain();
	initLight();
	initTexture();
	initShader();

	
	//initialise flock fl,  with geo_sheep as boid's geometry
	init_flock(&herd, geo_sheep, &hay_set);
	
	//SIMPLE FLOCK WITH HAY OFF TO SIDE
	/*
	add_boid (&herd, 5.0f, 0.0f, 1, 0);
	add_boid (&herd, -5.0f, 0.0f, -1, 0);
	add_boid (&herd, 0.0f, 5.0f, 0, 1);
	add_boid (&herd, 0.0f, -5.0f, 0, -1);

	create_affector(&hay_set, geo_hay, 1, 20.0f, 10.0f);
*/
	//MORE COMPLEX FLOCK WITH 2 HAYS
	add_boid (&herd, 5.1f, 0.1f, 1, 0);
	add_boid (&herd, -5.1f, 0.1f, -1, 0);
	add_boid (&herd, 0.1f, 5.1f, 0, 1);
	add_boid (&herd, 0.1f, -5.1f, 0, -1);

	add_boid (&herd, 3.0f, -1.0f, 1, 0);
	add_boid (&herd, -8.0f, 0.3f, -1, 0);
	add_boid (&herd, 0.0f, 12.0f, 0, 1);
	add_boid (&herd, -3.0f, -10.0f, 0, -1);

	create_affector (&hay_set, geo_hay, 1, 0.0f, 0.0f);
	create_affector (&hay_set, geo_hay, 1, 20.0f, 10.0f);
	create_affector (&hay_set, geo_hay, 1, 30.0f, 20.0f);



	// Loop until the user closes the window
	while (!glfwWindowShouldClose(g_window)) {

		// Make sure we draw to the WHOLE window
		int width, height;
		glfwGetFramebufferSize(g_window, &width, &height);

		// Main Render
		render(width, height);

		// Swap front and back buffers
		glfwSwapBuffers(g_window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
}





//-------------------------------------------------------------
// Fancy debug stuff
//-------------------------------------------------------------

// function to translate source to string
string getStringForSource(GLenum source) {

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return("API");
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return("Window System");
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return("Shader Compiler");
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return("Third Party");
	case GL_DEBUG_SOURCE_APPLICATION:
		return("Application");
	case GL_DEBUG_SOURCE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// function to translate severity to string
string getStringForSeverity(GLenum severity) {

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return("HIGH!");
	case GL_DEBUG_SEVERITY_MEDIUM:
		return("Medium");
	case GL_DEBUG_SEVERITY_LOW:
		return("Low");
	default:
		return("n/a");
	}
}

// function to translate type to string
string getStringForType(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		return("Error");
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return("Deprecated Behaviour");
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return("Undefined Behaviour");
	case GL_DEBUG_TYPE_PORTABILITY:
		return("Portability Issue");
	case GL_DEBUG_TYPE_PERFORMANCE:
		return("Performance Issue");
	case GL_DEBUG_TYPE_OTHER:
		return("Other");
	default:
		return("n/a");
	}
}

// actually define the function
void APIENTRY debugCallbackARB(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, 
	const GLchar* message, GLvoid*) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) return;

	cerr << endl; // extra space

	cerr << "Type: " <<
		getStringForType(type) << "; Source: " <<
		getStringForSource(source) << "; ID: " << id << "; Severity: " <<
		getStringForSeverity(severity) << endl;

	cerr << message << endl;

	if (type == GL_DEBUG_TYPE_ERROR_ARB) throw runtime_error("");
}
