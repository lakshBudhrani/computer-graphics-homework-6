// Author: Budhrani, Laksh Anil
// File: HW06.cpp
// Purpose: Creating a scene to simulate a simplified solar system

#include <iostream>
#include <cstdlib>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;

// Global variables for rotation and translation
static int sunRot = 0.0f, earthRot = 0, earthRev = 0, marsRot = 0, marsRev = 0, moonRot = 0, moonRev = 0;
bool isAnimation = false;
bool leftButtonDown = false;
bool middleButtonDown = false;
bool rightButtonDown = false;
float previousX = 0.0, previousY = 0.0, previousZ = 0.0;
float translateX = 0.0, translateY = 0.0, translateZ = 0.0;
float rotX = 0.0, rotY = 0.0, rotZ = 0.0;
float scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
float marsYear = 0;

// Function declarations
void init(void);
void instruction();
void display(void);
void reshape(int w, int h);
void initLighting(void);
void enableNormalization();
void setLightSourceProperties();
void setLightingModelProperties();
void enableLighting();
void renderString(const char* str);
void applyTransformations(float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
void defineMaterialProperties(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess[]);
void drawPlanet(float radius, int slices, int stacks, float color[]);
void myTimerFunc(int value);
void drawOrbits(double rad);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void myKeyboard(unsigned char key, int x, int y);

// Main routine
int main(int argc, char** argv) {
	// Print user instructions
	instruction();

	// Initialize GLUT
	glutInit(&argc, argv);

	// Set display mode to double buffer and RGB color
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Set window size and position
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);

	// Create window with given title
	glutCreateWindow("CSCI B365 - Final Project");

	// Initialize lighting
	init();

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(100, myTimerFunc, 0);

	// Enter GLUT main loop
	glutMainLoop();

	return 0;
}

// Input:  Nothing
// Output: Nothing
// Actions: Print instructions in the standard output window
void instruction() {
	cout << "Press and hold the left mouse button and move the mouse to rotate the scene.\n";
	cout << "Press and hold the middle mouse button and move the mouse up or down to scale the scene.\n";
	cout << "Press and hold the right mouse button and move the mouse to translate the scene.\n";
	cout << "Press 'r' or 'R' to reset all transformations to their initial values.\n";
	cout << "Press ' ' (space) to toggle animation on or off.\n";
	cout << "Press 'l' or 'L' to turn on the lights.\n";
	cout << "Press 's' or 'S' to turn off the lights.\n";
	cout << "Press 'Esc' to quit the program.\n";
}

// What: A function to do some one-time jobs
// Input: None
// Output: None
// Actions: Perform some one-time jobs, e.g., initializations
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); // Set the clear color to black
	glShadeModel(GL_SMOOTH); // Enable smooth shading

	enableNormalization(); // Enable automatic normalization of normal vectors
	setLightSourceProperties(); // Set light source properties
	setLightingModelProperties(); // Set lighting model properties
	enableLighting(); // Enable lighting
}

// What: A display call-back function that defines the picture that will be displayed.
// Action: 
//			1. Clear and then display the background color of the display window;
//			2. ... (Something specific to this program such as picture definitions);
//			3. Flush the buffer to display the image into the display window, i.e., show the image.
// Input: None
// Output: None
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	// Set the position of the text in the window using the x and y coordinates
	glRasterPos3f(-3, 0, -6);

	// Define the text color
	glColor3f(1.0, 1.0, 1.0); // White color

	// Render the string
	const char* str = "A simple solar system";
	renderString(str);

	glPushMatrix();
	applyTransformations(rotX, rotY, rotZ, scaleX, scaleY, scaleZ);

	// Define material properties for the sun
	GLfloat sun_ambient[] = { 0.3, 0.3, 0.0, 1.0 };
	GLfloat sun_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat sun_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat sun_shininess[] = { 50.0 };
	defineMaterialProperties(sun_ambient, sun_diffuse, sun_specular, sun_shininess);

	// Transformations of the large planet
	// Rotate the large sphere about its own y-axis
	glRotatef((GLfloat)sunRot, 0.0, 1.0, 0.0);

	// Draw a larger sphere
	// The sphere's center is at the modeling coordinates origin
	GLfloat sun_color[] = { 1.0, 1.0, 0.0 };
	drawPlanet(1.0, 50, 50, sun_color);   /* draw the sun */
	glPopMatrix();

	glPushMatrix();
	applyTransformations(rotX, rotY, rotZ, scaleX, scaleY, scaleZ);

	// Define material properties for the earth
	GLfloat earth_ambient[] = { 0.2, 0.6, 0.6, 1.0 };
	GLfloat earth_diffuse[] = { 0.3, 0.8, 0.8, 1.0 };
	GLfloat earth_specular[] = { 0.3, 1.0, 1.0, 1.0 };
	GLfloat earth_shininess[] = { 50.0 };
	defineMaterialProperties(earth_ambient, earth_diffuse, earth_specular, earth_shininess);

	// Transformations of the small planet - Earth
	drawOrbits(3.0);
	glRotatef((GLfloat)earthRev, 0.0, 1.0, 0.0);
	glTranslatef(3.0, 0.0, 0.0);
	glRotatef((GLfloat)earthRot, 0.0, 1.0, 0.0);

	drawOrbits(0.8);
	GLfloat earth_color[] = { 0.1843, 0.4156, 0.45 };
	drawPlanet(0.45, 30, 30, earth_color);    /* draw the object, a smaller planet */

	glPushMatrix();
	// Define material properties for the moon
	GLfloat moon_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat moon_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	GLfloat moon_specular[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat moon_shininess[] = { 50.0 };
	defineMaterialProperties(moon_ambient, moon_diffuse, moon_specular, moon_shininess);

	// Transformations for the small planet - Moon
	moonRev = -2 * earthRot / 3;
	glRotatef((GLfloat)moonRev, 0.0, 1.0, 0.0);
	glTranslatef(0.8, 0.0, 0.0);
	glRotatef((GLfloat)moonRot, 0.0, 1.0, 0.0);

	GLfloat moon_color[] = { 0.8, 0.8, 0.8 };
	drawPlanet(0.18, 10, 10, moon_color);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	applyTransformations(rotX, rotY, rotZ, scaleX, scaleY, scaleZ);

	// Define material properties for Mars
	GLfloat mars_ambient[] = { 0.6, 0.25, 0.0, 1.0 };
	GLfloat mars_diffuse[] = { 0.8, 0.3, 0.0, 1.0 };
	GLfloat mars_specular[] = { 1.0, 0.5, 0.0, 1.0 };
	GLfloat mars_shininess[] = { 50.0 };
	defineMaterialProperties(mars_ambient, mars_diffuse, mars_specular, mars_shininess);

	// Transformations of the small planet - Mars
	drawOrbits(5.0);
	glRotatef((GLfloat)marsRev, 0.0, 1.0, 0.0);
	glTranslatef(5.0, 0.0, 0.0);
	glRotatef((GLfloat)marsRot, 0.0, 1.0, 0.0);

	GLfloat mars_color[] = { 0.6, 0.2392, 0.0 };
	drawPlanet(0.3, 30, 30, mars_color);    /* draw the object, a smaller planet */
	glPopMatrix();

	glutSwapBuffers();
}

// What: A call-back function specifies the actions that will be taken when the display window is created or its size is changed.
// Input: 
//			1. w - Current display window's width (in pixels)
//			2. h - Current display window's height (in pixels)
// Output: None
// Actions: Perform some one-time jobs, e.g., initializations, such as:
//			1. Set up the viewport (whole or part of the display window)
//          2. Select and initialize the projection matrix stack used for projections;
//          3. Set up projection matrix, i.e., the viewing volume that has 6 clipping planes;
//          4. Select and initialize the model-view matrix stack used for geometric transformations;
//          5. Other possible one time jobs.
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Set the viewport to the full window size
	glMatrixMode(GL_PROJECTION); // Switch to setting the camera perspective
	glLoadIdentity(); // Reset the camera
	gluPerspective(70.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); // Set the perspective (angle of sight, width to height ratio, near and far planes)
	glMatrixMode(GL_MODELVIEW); // Switch to setting the model view
	glLoadIdentity(); // Reset the model view
	gluLookAt(0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0); // Set the camera (view point, aim point, up direction)
}


// Other Functions

// Functions related to Initialization

// What: Function to enable smooth shading and lighting
// Input: None
// Output: None
void initLighting(void) {
	// Set the clear color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Enable smooth shading model
	glShadeModel(GL_SMOOTH);

	// Light source properties
	GLfloat position[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

	// Lighting model properties
	GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat local_view[] = { 0.0 };

	// Enable automatic normalization of normal vectors
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	// Set light source properties
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Set lighting model properties
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	// Enable light source 0
	glEnable(GL_LIGHT0);

	// Enable lighting calculations
	glEnable(GL_LIGHTING);
}

// What: Function to enable automatic normalization of normal vectors
// Input: None
// Output: None 
void enableNormalization() {
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
}

// What: Function to set light source properties
// Input: None
// Output: None
void setLightSourceProperties() {
	GLfloat position[] = { 0.0, 20.0, 0.0, 1.0 }; // w=1 --> a positional source
	GLfloat ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

// What: Function to set lighting model properties
// Input: None
// Output: None 
void setLightingModelProperties() {
	GLfloat lmodel_ambient[] = { 0.5, 0.5, 0.5, 1.0 };	// OpenGL default values
	GLfloat local_view[] = { 0.0 }; // 0 or 0.0 implies that specular reflection angles take the view direction to be parallel to and in the direction of the -z axis

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
}

// What: Function to enable lighting
// Input: None
// Output: None
void enableLighting() {
	glEnable(GL_LIGHT0);		// Turn on the light 0
	glEnable(GL_LIGHTING);	// Enable the OpenGL lighting routine
}


// Functions related to Display

// What: Function to render the string
// Input: const char* str
// Output: None
void renderString(const char* str) {
	int len = (int)strlen(str);
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

// What: Function to apply transformations
// Input: float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ
// Output: None
void applyTransformations(float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ) {
	glTranslatef(translateX, translateY, translateZ);
	glRotatef(rotX, 1.0f, 0.0f, 0.0f);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
	glScalef(scaleX, scaleY, scaleZ);
}

// What: Function to define material properties
// Input: GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess[]
// Output: None
void defineMaterialProperties(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess[]) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

// What: Function to draw a planet
// Input: float radius, int slices, int stacks, float color[]
// Output: None
void drawPlanet(float radius, int slices, int stacks, float color[]) {
	glColor3fv(color);
	glutSolidSphere(radius, slices, stacks);
}

// What: Function to update rotation and revolution values periodically
// Input: int value
// Output: None
void myTimerFunc(int value) {
	// If animation is enabled, update rotation and revolution values
	if (isAnimation == true) {
		sunRot = (sunRot + 5) % 360;
		earthRot = (earthRot + 10);
		moonRot = (moonRot + 5) % 360;
		marsRot = (marsRot + 10) % 360;
		earthRev = (earthRev + 3) % 360;
		marsRev = (marsRev + 2) % 360;
	}

	// Redisplay the scene
	glutPostRedisplay();

	// Reset the timer
	glutTimerFunc(100, myTimerFunc, 0);
}

// What: Function to draw orbits
// Input: double rad
// Output: None
void drawOrbits(double rad) {
	// Save the current lighting state
	GLboolean lightingEnabled;
	glGetBooleanv(GL_LIGHTING, &lightingEnabled);

	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0); // Set the color to white
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float degInRad = i * 3.14159 / 180;
		glVertex3f(cos(degInRad) * rad, 0, sin(degInRad) * rad);
	}
	glEnd();

	// Restore the lighting state
	if (lightingEnabled) {
		glEnable(GL_LIGHTING);
	}
}


// Functions to take actions on Events

// What: Function to handle mouse input
// Input: int button, int state, int x, int y
// Output: None
void myMouse(int button, int state, int x, int y) {
	// Record button status
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftButtonDown = true;
	}
	else {
		leftButtonDown = false;
	}

	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
		middleButtonDown = true;
	}
	else {
		middleButtonDown = false;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rightButtonDown = true;
	}
	else {
		rightButtonDown = false;
	}

	// Update the start point's coordinates
	previousX = (float)x;
	previousY = (float)y;
	previousZ = (float)y;
}

// What: Function to handle mouse motion
// Input: int x, int y
// Output: None
void myMotion(int x, int y) {
	// Update transformation variables' values
	if (leftButtonDown) { // Rotate
		rotX += (y - previousY);
		rotY += (x - previousX);
	}

	if (middleButtonDown) { // Scale
		scaleX -= (y - previousY) * 0.005;
		scaleY -= (y - previousY) * 0.005;
		scaleZ -= (y - previousY) * 0.005;
	}

	if (rightButtonDown) { // Translate
		translateX += (x - previousX) * 0.1;
		translateY -= (y - previousY) * 0.1;
	}

	// Update the start point's coordinates
	previousX = (float)x;
	previousY = (float)y;
	previousZ = (float)y;

	// Recall the display call-back function to update the display
	glutPostRedisplay();
}

// What: Function to handle keyboard input
// Input: unsigned char key, int x, int y
// Output: None
void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
	case 'R':
		// Reset all transformation variables' value to be initial values
		translateX = 0.0, translateY = 0.0, translateZ = 0.0;
		rotX = 0.0, rotY = 0.0, rotZ = 0.0;
		scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
		break;
	case ' ':
		// Toggle animation
		isAnimation = !isAnimation;
		break;
	case 'l':
	case 'L':
		// Turn on the light(s)
		glEnable(GL_LIGHTING);
		break;
	case 's':
	case 'S':
		// Turn off the light(s)
		glDisable(GL_LIGHTING);
		break;
	case 27: // ASCII value for 'Esc' key
		// Quit the program
		exit(0);
		break;
	default:
		break;
	}

	// Recall the display call-back function
	glutPostRedisplay();
}