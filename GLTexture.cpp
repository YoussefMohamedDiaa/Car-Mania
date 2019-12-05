#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

double angleView = 0;
double forward = 0;
double sideMove = 0;
double sideAngle = 0;

double downfp = 0;
double forwardfp = 0;
double upFp = 0;
double rightFp = 0;
double eyeFp = 0;


int ground1Far = -120;
int ground1Near = 20;
int ground2Far = 20;
int ground2Near = 160;

bool nextDone = false;
bool nextDone2 = false;
bool fp = false;
class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(2, 5, 10);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_car;
Model_3DS model_tree;
Model_3DS model_building;
Model_3DS model_building2;
Model_3DS model_barrier;
Model_3DS model_chair;

// Textures
GLTexture tex_ground;
GLTexture car_view;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	//gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}


//for first person
void RenderCarView()
{

	glPushMatrix();
	glTranslated(0 + sideMove, 0, 0 - forward);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1, 1, 0, 0.7);
	glBegin(GL_QUADS);
	glVertex3f(5, 0.5, -0.2);
	glVertex3f(5, 0.5, -0.2);
	glVertex3f(2.7, 0.1, -4);
	glVertex3f(5.5, 0.1, -4);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3f(5.8-0.2, 0.5, -0.2);
	glVertex3f(5.8 - 0.2, 0.5, -0.2);
	glVertex3f(5.5 - 0.2, 0.1, -4);
	glVertex3f(8.3 - 0.2, 0.1, -4);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0 + sideMove, 0, 0 - forward);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0, 0, 0, 0.2);
	glBegin(GL_QUADS);
	glVertex3f(-10, 0, -1);
	glVertex3f(10,0, -1);
	glVertex3f(10, 7, -1);
	glVertex3f(-10, 7, -1);
	glEnd();
	glPopMatrix();

	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, car_view.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glTranslated(0 + sideMove, 0, 0 - forward);
	glTranslated(4.77, 0.4, 0);
	glScaled(0.45, 0.12, 0.5);
	glBegin(GL_QUADS);
	glDisable(GL_REPEAT);
	glNormal3f(0, 0, 1);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0);
	glVertex3f(2, 0,0);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2,0);
	glTexCoord2f(0, 1);
	glVertex3f(0, 2,0);
	glEnd();
	glPopMatrix();


	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderGround()
{

	int cur = 20;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(2, 0, ground1Far+ cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(3, 0, ground1Far+cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(4, 0, ground1Far+cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();
		cur += 20;
	}

	 cur = 10;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(2, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(3, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(4, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();
		cur += 20;
	}

	/*
	glPushMatrix();
	glTranslated(-20, 0, ground1Far);
	glScaled(0.1, 0.18, 1);
	glRotated(-90, 0, 1, 0);
	model_building2.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20, 0, ground1Far);
	glScaled(0.1, 0.18, 1);
	glRotated(90, 0, 1, 0);
	model_building2.Draw();
	glPopMatrix();
	*/

	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, ground1Far);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, ground1Far);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, ground1Near);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, ground1Near);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderGround2()
{

	int cur = 20;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(-2, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-3, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-4, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();


		cur += 20;
	}



	 cur = 10;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(2, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(3, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		glPushMatrix();
		glTranslated(4, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();
		cur += 20;
	}

	/*
	glPushMatrix();
	glTranslated(-20, 0, ground2Far);
	glScaled(0.1, 0.18, 1);
	glRotated(-90, 0, 1, 0);
	model_building2.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20, 0, ground2Far);
	glScaled(0.1, 0.18, 1);
	glRotated(90, 0, 1, 0);
	model_building2.Draw();
	glPopMatrix();
	*/

	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, ground2Far);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, ground2Far);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, ground2Near);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, ground2Near);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Display Function
//=======================================================================


void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-4, 4, -4, 4, -5, 5);
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5 + sideMove+ eyeFp, 1.5-downfp, 3.5 - forward-forwardfp, 5 + sideMove+rightFp, 0+ upFp, 0.0 - forward, 0.0, 1.0, 0.0);
}
void myDisplay(void)
{

	setupCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);


	//ground
	RenderGround();
	RenderGround2();
	
	
	//first person
	if(fp)
	RenderCarView();
	// Draw house Model


	//car
	glPushMatrix();
	glTranslated(-0.5 + sideMove, 0, 0 - forward);
	glTranslated(5, 0, 0);
	glRotated(-90, 0, 1, 0);
	glRotated(sideAngle, 0, 1, 0);
	glTranslated(-5, 0, 0);
	if(!fp)
	 model_car.Draw();
	glPopMatrix();



	glutSwapBuffers();
}




//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5 + sideMove+eyeFp, 1.5-downfp, 3.5 - forward- forwardfp, 5 + sideMove+rightFp, 0+ upFp, 0.0 - forward, 0.0, 1.0, 0.0);
}



//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_car.Load("Models/house/house.3ds");
	model_chair.Load("Models/house/chair.3ds");
	model_barrier.Load("Models/house/barrier.3ds");
	//model_building.Load("Models/house/building.3ds");
	model_building2.Load("Models/house/building2.3ds");
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	car_view.Load("Textures/carView.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================

void Anim()
{
	//first person
	if (fp) {

		 downfp = 0.7;
		 forwardfp = 2.8;
		 upFp = 0.8;
		 rightFp = 0.16;
		 eyeFp = 0.1;
	}
	else {
		downfp = 0;
		forwardfp = 0;
		upFp = 0;
		rightFp = 0;
		eyeFp = 0;
	}

	//moving side
	if (sideAngle > 0)
		sideAngle -= 0.02;
	else if (sideAngle < 0)
		sideAngle += 0.02;


	angleView += 0.1;
	forward += 0.5;

	//next ground
	if (-forward < ground1Far + 100 && -forward > ground1Far) {
		ground2Far = ground1Far - 140;
		ground2Near = ground1Far;

	}
	else if (-forward < ground2Far + 100 && -forward > ground2Far) {
		ground1Far = ground2Far - 140;
		ground1Near = ground2Far;

	}
	glutPostRedisplay();
}

void keyboardFunc(int key, int x, int y) {

	switch (key) {
    case GLUT_KEY_UP:
		fp = true;
			break;
	case GLUT_KEY_DOWN:
		fp = false;
		break;
	case GLUT_KEY_LEFT:
		sideMove -= 0.1;
		sideAngle = 10;
		break;
	case GLUT_KEY_RIGHT:
		sideMove += 0.1;
		sideAngle = -10;
		break;
	}

}


void main(int argc, char** argv)
{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);



	//glutKeyboardFunc(myKeyboard);

	//glutMotionFunc(myMotion);

	//glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glutSpecialFunc(keyboardFunc);
	glutIdleFunc(Anim);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
