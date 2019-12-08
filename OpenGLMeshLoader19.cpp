#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <vector>

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
double sideMove = -2;
double sideAngle = 0;

double downfp = 0;
int crashCount = 0; //might change it to long

int ground1Far = -120;
int ground1Near = 20;
int ground2Far = 20;
int ground2Near = 160;
int repititions = 4;
double forwardfp = 0;
double upFp = 0;
double rightFp = 0;
double eyeFp = 0;

double carMinX = 0.0;
double carMaxX = 0.0;
double carMinY = 0.0;
double carMaxY = 0.0;
double carMinZ = 0.0;
double carMaxZ = 0.0;


int printCount = 0;

bool nextDone = false;
bool nextDone2 = false;
bool fp = false;
bool scene1 = true;

int score = 0;
int barrierCounter = 0;
int lastPrintedScore = 0;

double forwardSpeed = 0.5;

double incrementScore = 0;
double prevSideMove = 0;
double prevForwardMove = 0;

int lives = 3;

bool gameOver = false;

double jump = 0;
double jumpAngle = 0;
bool ramp = false;
bool upJump = true;

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
Model_3DS model_powerLine;

// Textures
GLTexture tex_ground;
GLTexture car_view;
GLTexture tex_wood;

void print(int x, int y, int z, char* string)
{
	int len, i;

	glRasterPos3f(x, y, z);

	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void keyboardOtherButtons(unsigned char key, int x, int y) {
	switch (key)
	{
	case 's': scene1 = !scene1;
		if (scene1) {

			tex_ground.Load("Textures/ground.bmp");
		}
		else {

			tex_ground.Load("Textures/ground1.bmp");
		}
		break;
	default:
		break;
	}
}

void light(short lightNumber, std::vector<float> position, std::vector<float> direction, int lightDistribution, int spreadAngle) {

	float modelAmbient = 0.1, ambient = 1;
	GLfloat model_ambient[] = { modelAmbient, modelAmbient, modelAmbient, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

	GLfloat lDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lAmbient[] = { ambient, 0.0f, 0.0f, 1.0f };
	GLfloat lPosition[] = { position[0], position[1], position[2], true };
	GLfloat lDirection[] = { direction[0], direction[1], direction[2] };
	GLfloat lSpecular[] = { 1, 0, 0, 1.0f };

	glLightfv(lightNumber, GL_DIFFUSE, lDiffuse);
	glLightfv(lightNumber, GL_AMBIENT, lAmbient);
	glLightfv(lightNumber, GL_SPECULAR, lSpecular);
	glLightfv(lightNumber, GL_POSITION, lPosition);
	glLightf(lightNumber, GL_SPOT_EXPONENT, lightDistribution);
	glLightf(lightNumber, GL_SPOT_CUTOFF, spreadAngle);
	glLightfv(lightNumber, GL_SPOT_DIRECTION, lDirection);
}


void checkCrash(int coneNum, double x, double z) {
	if (!gameOver) {
		if (x + 0.2 <= carMaxX && x >= carMinX && z <= (carMaxZ - 1.6) && z >= (carMinZ + 1.6)) {
			mciSendString("play \"\crash.wav\"", NULL, 0, NULL);
			lives--;
			if (lives == 0) {
				//lose
			}
			gameOver = true;
		}
		else {
			barrierCounter++;
			if (barrierCounter >= 5000) {
				score++;
				if (score == 30)
					forwardSpeed += 0.2;
				barrierCounter = 0;
			}
		}
	}
	else {
		char* finalScoreArray[20];
		sprintf((char*)finalScoreArray, "Final Score : %d", score);
		print(6.6 + sideMove, 1.2, -0.5 - forward, (char*)(finalScoreArray));
	}
}

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
	//printf("%f %f\n", 2.7+sideMove + 0.5, 5.5+sideMove + 0.5);

	glBegin(GL_QUADS);
	glVertex3f(5.8 - 0.2, 0.5, -0.2);
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
	glVertex3f(10, 0, -1);
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
	glVertex3f(2, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(2, 2, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, 2, 0);
	glEnd();
	glPopMatrix();


	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	//glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit
	
	int cur = 20;
	for (int i = 0; i < 10; i++) {
		if (i % 2 == 0) {

			for (int i = 0; i < repititions; i++) {
				if (scene1) {
					glPushMatrix();
					glTranslated(-1 * i, 0, ground1Far + cur);
					glScaled(0.2, 0.2, 0.2);
					model_barrier.Draw();
					glPopMatrix();
				}
				checkCrash((-1 * i) + 4, -1 * i, ground1Far + cur);
			}
			if (!scene1) {
				glPushMatrix();
				glTranslated(-1.25, 0, ground1Far + cur);
				glScaled(0.008, 0.008, 0.008);
				model_chair.Draw();
				glPopMatrix();
			}

		}
		else {
			for (int i = 0; i < repititions; i++) {
				if (scene1) {
					glPushMatrix();
					glTranslated(i, 0, ground1Far + cur);
					glScaled(0.2, 0.2, 0.2);
					model_barrier.Draw();
					glPopMatrix();
				}
				checkCrash(i + 4, i, ground1Far + cur);
			}
			if (!scene1) {
				glPushMatrix();
				glTranslated(1.7, 0, ground1Far + cur);
				glScaled(0.008, 0.008, 0.008);
				model_chair.Draw();
				glPopMatrix();
			}

		}
		cur += 45;
	}

	if (scene1) {
		cur = 10;
		for (int i = 0; i < 25; i++) {
			glPushMatrix();
			glTranslated(-20, 0, ground1Far + cur);
			glScaled(0.04, 0.02, 0.02);
			model_powerLine.Draw();
			glPopMatrix();

			glPushMatrix();
			glTranslated(20, 0, ground1Far + cur);
			glScaled(0.04, 0.02, 0.02);
			glRotated(-180, 0, 1, 0);
			model_powerLine.Draw();
			glPopMatrix();
			cur += 20;
		}
	}

	cur = 20;
	for (int i = 0; i < 10; i++) {
		if (i % 2 != 0) {
			for (int i = 0; i < repititions; i++) {
				if (scene1) {
					glPushMatrix();
					glTranslated(-1 * (i + 5), 0, ground1Far + cur);
					glScaled(0.2, 0.2, 0.2);
					model_barrier.Draw();
					glPopMatrix();
				}
				checkCrash(i + 3, -1 * (i + 5), ground1Far + cur);
			}
			if (!scene1) {
				glPushMatrix();
				glTranslated(-6.2, 0, ground1Far + cur);
				glScaled(0.008, 0.008, 0.008);
				model_chair.Draw();
				glPopMatrix();
			}

		}
		else {
			for (int i = 0; i < repititions; i++) {
				if (scene1) {
					glPushMatrix();
					glTranslated((i + 5), 0, ground1Far + cur);
					glScaled(0.2, 0.2, 0.2);
					model_barrier.Draw();
					glPopMatrix();
				}
				checkCrash(i + 4, (i + 5), ground1Far + cur);
			}
			if (!scene1) {
				glPushMatrix();
				glTranslated(6.6, 0, ground1Far + cur);
				glScaled(0.008, 0.008, 0.008);
				model_chair.Draw();
				glPopMatrix();
			}


		}
		cur += 45;
	}

	cur = 20;
	for (int i = 0; i < 10; i++) {
		for (int i = 0; i < repititions; i++) {
			if (scene1) {
				glPushMatrix();
				glTranslated((i + 10) * -1, 0, ground1Far + cur);
				glScaled(0.2, 0.2, 0.2);
				model_barrier.Draw();
				glPopMatrix();
			}
			checkCrash(i, (i + 10) * -1, ground1Far + cur);
		}
		if (!scene1) {
			glPushMatrix();
			glTranslated(11.7, 0, ground1Far + cur);
			glScaled(0.008, 0.008, 0.008);
			model_chair.Draw();
			glPopMatrix();
		}
		//=============================================//
		for (int i = 0; i < repititions; i++) {
			if (scene1) {
				glPushMatrix();
				glTranslated(i + 10, 0, ground1Far + cur);
				glScaled(0.2, 0.2, 0.2);
				model_barrier.Draw();
				glPopMatrix();
			}
			checkCrash(4, i + 10, ground1Far + cur);

		}
		if (!scene1) {
			glPushMatrix();
			glTranslated(-11, 0, ground1Far + cur);
			glScaled(0.008, 0.008, 0.008);
			model_chair.Draw();
			glPopMatrix();
		}
		cur += 45;
	}

	/*cur = 10;
	for (int i = 0; i < 10; i++) {
		glPushMatrix();
		glTranslated(2, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		checkCrash(4, 2, ground1Far + cur);

		glPushMatrix();
		glTranslated(3, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		checkCrash(5, 3, ground1Far + cur);

		glPushMatrix();
		glTranslated(4, 0, ground1Far + cur);
		glScaled(0.2, 0.2, 0.2);
		model_barrier.Draw();
		glPopMatrix();

		checkCrash(6, 4, ground1Far + cur);

		cur += 20;
	}*/

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


	glDisable(GL_LIGHTING);	// Disable lighting 

	//glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	if (scene1) {
		int cur = 10;
		for (int i = 0; i < 25; i++) {
			glPushMatrix();
			glTranslated(-20, 0, ground2Far + cur);
			glScaled(0.04, 0.02, 0.02);
			model_powerLine.Draw();
			glPopMatrix();

			glPushMatrix();
			glTranslated(20, 0, ground2Far + cur);
			glScaled(0.04, 0.02, 0.02);
			glRotated(-180, 0, 1, 0);
			model_powerLine.Draw();
			glPopMatrix();
			cur += 20;
		}

		glPushMatrix();
		glTranslated(0, 0, ground2Far + 80);
		glScaled(0.2, 0.15, 0.1);
		glRotated(-180, 0, 1, 0);
		model_building2.Draw();
		glPopMatrix();

		//ramp
		glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

		glBindTexture(GL_TEXTURE_2D, tex_wood.texture[0]);	// Bind the ground texture

		glPushMatrix();
		glTranslated(0, 0, (ground2Far + 80 + 49));
		glRotated(30, 1, 0, 0);
		glTranslated(0, 0, -(ground2Far + 80 + 49));
		glBegin(GL_QUADS);
		glNormal3f(0, 1, 0);	// Set quad normal direction.
		glTexCoord2f(0, 0);
		glVertex3f(-2, 0, ground2Far + 80 + 34);
		glTexCoord2f(5, 0);
		glVertex3f(2, 0, ground2Far + 80 + 34);
		glTexCoord2f(5, 5);
		glVertex3f(2, 0, ground2Far + 80 + 64);
		glTexCoord2f(0, 5);
		glVertex3f(-2, 0, ground2Far + 80 + 64);
		glEnd();
		glPopMatrix();
	}


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
	gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5 + sideMove + eyeFp, 1.5 - downfp + jump, 3.5 - forward - forwardfp, 5 + sideMove + rightFp, 0 + upFp + jump, 0.0 - forward, 0.0, 1.0, 0.0);
	//gluLookAt(0, 0, 1, 0, 0, 0, 0.0, 1.0, 0.0);
}


void myDisplay(void)
{
	glColor3f(1.0, 1, 1);
	setupCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	carMinX = 4.3 + sideMove;
	carMaxX = 5.5 + sideMove;
	carMinY = 0.1;
	carMaxY = 0.5;
	carMinZ = -4 - forward;
	carMaxZ = -0.2 - forward;

	//ground
	RenderGround();
	RenderGround2();

	if (score - lastPrintedScore >= 10 || lastPrintedScore == 0) {
		prevSideMove = sideMove;
		prevForwardMove = forward;

		lastPrintedScore = score;
	}
	else {
		char* scoreArray[20];
		sprintf((char*)scoreArray, "Score : %d", score);
		print(6.6 + prevSideMove, 1.2, -0.5 - prevForwardMove - 100, (char*)(scoreArray));
	}



	//first person
	if (fp)
		RenderCarView();
	// Draw house Model



	//car
	glPushMatrix();
	glTranslated(-0.5 + sideMove, jump, 0 - forward);
	glTranslated(5, 0, 0);
	glRotated(jumpAngle, 1, 0, 0);
	glRotated(-90, 0, 1, 0);
	glRotated(sideAngle, 0, 1, 0);
	glTranslated(-5, 0, 0);
	if (!fp) {
		model_car.Draw();
	}
	glPopMatrix();
	/*
		glBegin(GL_QUADS);
		glVertex3f(-3, 0, -5);
		glVertex3f(-3, 0, -10);
		glVertex3f(3, 0, -10);
		glVertex3f(3, 0, -5);
		glEnd();
	*/
	//glutSolidCube(0.25);

	//light(GL_LIGHT1, { 0, 0, -1 }, { 0, 0, 1 }, 10, 90);
	///light(GL_LIGHT2, { (float)(-0.5 + sideMove), 0, (float)(-forward) }, { 0, 0, -1 }, 10, 90);
	//light(GL_LIGHT2, { (float)(-0.5 + sideMove), 1, (float)(-forward - 10) }, { 0, 0, -1 }, 10, 90);
	//light(GL_LIGHT2, { (float)(-0.5 + sideMove), 1, (float)(-forward) }, { 0, 0, -1 }, 10, 90);
	//light(GL_LIGHT2, { (float)(-0.5 + sideMove), 1, (float)(-forward - 20) }, { 0, 0, -1 }, 10, 90);
	light(GL_LIGHT2, { (float)(-0.5 + sideMove), 0, (float)(-forward + 30) }, { 0, 0, 1 }, 10, 120);

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
	//gluLookAt(5 + sideMove + eyeFp, 1.5 - downfp, 3.5 - forward - forwardfp, 5 + sideMove + rightFp, 0 + upFp, 0.0 - forward, 0.0, 1.0, 0.0);
	gluLookAt(5 + sideMove + eyeFp, 1.5 - downfp + jump, 3.5 - forward - forwardfp, 5 + sideMove + rightFp, 0 + upFp + jump, 0.0 - forward, 0.0, 1.0, 0.0);
	//gluLookAt(0, 0, 1, 0, 0, 0, 0.0, 1.0, 0.0);
}



//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{

	// Loading Model files
	model_car.Load("Models/house/house.3ds");


	//model_building.Load("Models/house/building.3ds");
	model_powerLine.Load("Models/house/powerLine.3ds");
	model_building2.Load("Models/house/building2.3ds");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_chair.Load("Models/house/chair.3ds");
	model_barrier.Load("Models/house/barrier.3ds");
	// Loading texture files
	if (scene1) {
		tex_ground.Load("Textures/ground.bmp");

	}
	else {

		tex_ground.Load("Textures/ground1.bmp");

	}
	tex_wood.Load("Textures/wood.bmp");
	car_view.Load("Textures/carView.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================

void Anim()
{

	if (scene1) {
		if (2 + 0.2 >= carMaxX && -2 - 0.2 <= carMinX && -forward > ground2Far + 80 + 45 && -forward < ground2Far + 80 + 50 && -forward<ground2Near && -forward > ground2Far)
			ramp = true;
		else if (-forward > ground2Far + 80 + 5 && -forward < ground2Far + 80 + 10 && -forward<ground2Near && -forward > ground2Far) {
			if(!jump)
			gameOver = true;
		}

		if (ramp) {
			if (upJump && jump < 30) {
				jump += 0.5;
				jumpAngle = 30;
			}
			else
			{
				upJump = false;
				if (jumpAngle > 0)
					jumpAngle -= 0.5;

				if (jump > 0)
					jump -= 0.5;
				else {
					ramp = false;
					upJump = true;
				}
			}
		}
	}

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
	if (!gameOver) {
		//if (scene1)
			forward += forwardSpeed;
		//else
			//forward += 4 * forwardSpeed;
	}

	incrementScore += 0.000000000001;
	if (incrementScore >= 1) {
		incrementScore = 0;
		score++;
	}

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
	if (!gameOver) {
		switch (key) {
		case GLUT_KEY_UP:
			fp = true;
			break;
		case GLUT_KEY_DOWN:
			fp = false;
			break;
		case GLUT_KEY_LEFT:
			sideMove = max(sideMove - 0.1, -24);
			sideAngle = 10;
			break;
		case GLUT_KEY_RIGHT:
			sideMove = min(sideMove + 0.1, 14);
			sideAngle = -10;
			break;
		}
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

	PlaySound(TEXT("car.wav"), NULL, SND_ASYNC | SND_LOOP);

	//glutKeyboardFunc(myKeyboard);

	//glutMotionFunc(myMotion);

	//glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);

	LoadAssets();
	glutSpecialFunc(keyboardFunc);
	glutIdleFunc(Anim);
	glShadeModel(GL_SMOOTH);
	glutKeyboardFunc(keyboardOtherButtons);
	glutMainLoop();
}
