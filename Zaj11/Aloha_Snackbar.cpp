#pragma warning(push) 
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 4838)
#pragma warning(disable: 4996)
#include <xutility>


#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include "colors.h"
#include "glext.h"
#define NUM_PARTICLES 1000 /* Number of particles */
#define NUM_DEBRIS 70 /* Number of debris */
/* GLUT menu entries */
#define PAUSE 0
#define NORMALIZE_SPEED 1
#define QUIT 2

PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL;

GLint fog_hint = GL_DONT_CARE;

// początek i koniec oddziaływania mgły liniowej

GLfloat fog_start = 5.0;
GLfloat fog_end = 10.0;

// gęstość mgły

GLfloat fog_density = 0.5;

// rodzaj mgły

GLfloat fog_mode = GL_LINEAR;


/* A particle */
struct particleData
{
	float position[3];
	float speed[3];
	float color[3];
};
typedef struct particleData particleData;
/* A piece of debris */
struct debrisData
{
	float position[3];
	float speed[3];
	float orientation[3]; /* Rotation angles around x, y, and z axes */
	float orientationSpeed[3];
	float color[3];
	float scale[3];
};
typedef struct debrisData debrisData;
/* Globals */
particleData particles[NUM_PARTICLES];
debrisData debris[NUM_DEBRIS];
int fuel = 0; /* "fuel" of the explosion */
float angle = 0.0; /* camera rotation angle */
				   /* Light sources and
				   material */
GLfloat light0Amb[4] = { 1.0, 0.6, 0.2, 1.0 };
GLfloat light0Dif[4] = { 1.0, 0.6, 0.2, 1.0 };
GLfloat light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light0Pos[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light1Amb[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light1Dif[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1Pos[4] = { 0.0, 5.0, 5.0, 0.0 };
GLfloat materialAmb[4] = { 0.25, 0.22, 0.26, 1.0 };
GLfloat materialDif[4] = { 0.63, 0.57, 0.60, 1.0 };
GLfloat materialSpec[4] = { 0.99, 0.91, 0.81, 1.0 };
GLfloat materialShininess = 27.8;
int wantNormalize = 0; /* Speed vector normalization flag */
int wantPause = 0; /* Pause flag */
				   /*
				   * newSpeed
				   *
				   * Randomize a new speed vector.
				   *
				   */

typedef float Vector3[3];
constexpr int verticesCount = 7;

Vector3 center[1] = { 0,0,0 };
Vector3 vCorners[verticesCount + 1];
void SetupPyramid(float radius) {
	//G�ra 0
	vCorners[0][0] = 0.0f;
	vCorners[0][1] = radius * 1.5;
	vCorners[0][2] = 0.0f;
	GLdouble degree = 2 * M_PI / verticesCount;
	for (int i = 1; i < verticesCount + 1; i++)
	{
		vCorners[i][0] = radius*cos(i*degree);
		vCorners[i][1] = 0;
		vCorners[i][2] = radius*sin(i*degree);
	}
}



void RenderPyramid() {
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	//glRotatef(-90, 1, 0, 0);
	//glTranslatef(0.0f, -0.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(0,0,1);

	//Podstawa piramidy
	for (int i = 1; i <= verticesCount; i++)
	{
		int j = i + 1;
		if (i == verticesCount)
			j = 1;
		glVertex3fv(center[0]);
		glVertex3fv(vCorners[i]);
		glVertex3fv(vCorners[j]);
	}

	//Boki piramidy
	for (int i = 1; i <= verticesCount; i++)
	{
		int j = i + 1;
		if (i == verticesCount)
			j = 1;

		glVertex3fv(vCorners[0]);
		glVertex3fv(vCorners[j]);
		glVertex3fv(vCorners[i]);
	}
	glEnd();
	glPopMatrix();
}

void DrawString(GLint x, GLint y, char * string)
{
	// położenie napisu
	//glWindowPos2i(x, y);

	// wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);

}


void newSpeed(float dest[3])
{
	float x;
	float y;
	float z;
	float len;
	x = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	y = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	z = (2.0 * ((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
	/*
	* Normalizing the speed vectors gives a "fireball" effect
	*
	*/
	if (wantNormalize)
	{
		len = sqrt(x * x + y * y + z * z);
		if (len)
		{
			x = x / len;
			y = y / len;
			z = z / len;
		}
	}
	dest[0] = x;
	dest[1] = y;
	dest[2] = z;
}
/*
* newExplosion
*
* Create a new explosion.
*
*/
void newExplosion(void)
{
	int i;
	for (i = 0; i < NUM_PARTICLES; i++)
	{
		particles[i].position[0] = 0.0;
		particles[i].position[1] = 0.0;
		particles[i].position[2] = 0.0;
		particles[i].color[0] = 1.0;
		particles[i].color[1] = 1.0;
		particles[i].color[2] = 0.5;
		newSpeed(particles[i].speed);
	}
	for (i = 0; i < NUM_DEBRIS; i++)
	{
		debris[i].position[0] = 0.0;
		debris[i].position[1] = 0.0;
		debris[i].position[2] = 0.0;
		debris[i].orientation[0] = 0.0;
		debris[i].orientation[1] = 0.0;
		debris[i].orientation[2] = 0.0;
		debris[i].color[0] = 0.7;
		debris[i].color[1] = 0.7;
		debris[i].color[2] = 0.7;
		debris[i].scale[0] = (2.0 *
			((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[1] = (2.0 *
			((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		debris[i].scale[2] = (2.0 *
			((GLfloat)rand()) / ((GLfloat)RAND_MAX)) - 1.0;
		newSpeed(debris[i].speed);
		newSpeed(debris[i].orientationSpeed);
	}
	fuel = 100;
}
/*
* display
*
* Draw the scene.
*
*/
void display(void)
{

	int i;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	/* Place the camera */
	glTranslatef(0.0, 0.0, -10.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	// włączenie efektu mgły
	glEnable(GL_FOG);

	// wskazówki jakości generacji mgły
	glHint(GL_FOG_HINT, fog_hint);

	// kolor mgły
	glFogfv(GL_FOG_COLOR, White);

	// gęstość mgły
	glFogf(GL_FOG_DENSITY, fog_density);

	// rodzaj mgły
	glFogf(GL_FOG_MODE, fog_mode);

	// początek i koniec oddziaływania mgły liniowej
	glFogf(GL_FOG_START, fog_start);
	glFogf(GL_FOG_END, fog_end);

	/* If no explosion, draw cube */
	if (fuel == 0)
	{
		glEnable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		//glutSolidCube(1.0);
		RenderPyramid();
	}
	if (fuel > 0)
	{
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
		for (i = 0; i < NUM_PARTICLES; i++)
		{
			glColor3fv(particles[i].color);
			glVertex3fv(particles[i].position);
		}
		glEnd();
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);
		glNormal3f(0.0, 0.0, 1.0);
		for (i = 0; i < NUM_DEBRIS; i++)
		{
			glColor3fv(debris[i].color);
			glPushMatrix();
			glTranslatef(debris[i].position[0],
				debris[i].position[1],
				debris[i].position[2]);
			glRotatef(debris[i].orientation[0], 1.0, 0.0, 0.0);
			glRotatef(debris[i].orientation[1], 0.0, 1.0, 0.0);
			glRotatef(debris[i].orientation[2], 0.0, 0.0, 1.0);
			glScalef(debris[i].scale[0],
				debris[i].scale[1],
				debris[i].scale[2]);
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0, 0.5, 0.0);
			glVertex3f(-0.25, 0.0, 0.0);
			glVertex3f(0.25, 0.0, 0.0);
			glEnd();
			glPopMatrix();
		}
	}

	// wyłączenie efektu mgły
	glDisable(GL_FOG);

	// wyświetlenie wybranych informacji
	char string[200];
	GLfloat var[4];
	glColor3fv(Black);

	// gęstość mgły
	glGetFloatv(GL_FOG_DENSITY, var);
	sprintf(string, "GL_FOG_DENSITY = %f", var[0]);
	DrawString(2, 2, string);

	// informacja o wskazówkach generacji mgły
	glGetFloatv(GL_FOG_HINT, var);
	switch ((int)var[0])
	{
	case GL_FASTEST:
		sprintf(string, "GL_FOG_HINT = GL_FASTEST");
		break;
	case GL_DONT_CARE:
		sprintf(string, "GL_FOG_HINT = GL_DONT_CARE");
		break;
	case GL_NICEST:
		sprintf(string, "GL_FOG_HINT = GL_NICEST");
		break;
	}
	DrawString(2, 16, string);

	// początek i koniec oddziaływania mgły liniowej
	glGetFloatv(GL_FOG_START, var);
	glGetFloatv(GL_FOG_END, var + 1);
	sprintf(string, "GL_FOG_START = %f GL_FOG_END = %f", var[0], var[1]);
	DrawString(2, 30, string);

	// rodzaj mgły
	glGetFloatv(GL_FOG_MODE, var);
	switch ((int)var[0])
	{
	case GL_LINEAR:
		sprintf(string, "GL_FOG_MODE = GL_LINEAR");
		break;
	case GL_EXP:
		sprintf(string, "GL_FOG_MODE = GL_EXP");
		break;
	case GL_EXP2:
		sprintf(string, "GL_FOG_MODE = GL_EXP2");
		break;
	}
	DrawString(2, 44, string);

	// skierowanie poleceń do wykonania
	glFlush();

	glutSwapBuffers();
}
/*
* keyboard
*
* Keyboard callback.
*
*/


void reshape(int w,
	int h)
{
	glViewport(0.0, 0.0, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w górę
	case GLUT_KEY_UP:
		fog_start += 0.1;
		fog_end += 0.1;
		break;

		// kursor w dół
	case GLUT_KEY_DOWN:
		fog_start -= 0.1;
		fog_end -= 0.1;
		break;
	}

	// odrysowanie okna
	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}


void keyboard(unsigned char key,
	int x,
	int y)
{


	switch (key)
	{
	case ' ':
		newExplosion();
		break;
	case 27:
		exit(0);
		break;
	case 'p':
		wantPause = 1 - wantPause;
		break;
	case '+':
			fog_density += 0.05;
			break;
	case '-':
		if(fog_density > 0.5)
			fog_density -= 0.05;
		break;
	}
}
/*
* idle
*
* Update animation variables.
*
*/



// funkcja rysująca napis w wybranym miejscu
// (wersja korzystająca z funkcji glWindowPos2i)






void idle(void)
{
	int i;
	if (!wantPause)
	{
		if (fuel > 0)
		{
			for (i = 0; i < NUM_PARTICLES; i++)
			{
				particles[i].position[0] += particles[i].speed[0] * 0.2;
				particles[i].position[1] += particles[i].speed[1] * 0.2;
				particles[i].position[2] += particles[i].speed[2] * 0.2;
				particles[i].color[0] -= 1.0 / 500.0;
				if (particles[i].color[0] < 0.0)
				{
					particles[i].color[0] = 0.0;
				}
				particles[i].color[1] -= 1.0 / 100.0;
				if (particles[i].color[1] < 0.0)
				{
					particles[i].color[1] = 0.0;
				}
				particles[i].color[2] -= 1.0 / 50.0;
				if (particles[i].color[2] < 0.0)
				{
					particles[i].color[2] = 0.0;
				}
			}
			for (i = 0; i < NUM_DEBRIS; i++)
			{
				debris[i].position[0] += debris[i].speed[0] * 0.1;
				debris[i].position[1] += debris[i].speed[1] * 0.1;
				debris[i].position[2] += debris[i].speed[2] * 0.1;
				debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
				debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
				debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
			}
			--fuel;
		}
		angle += 0.3; /* Always continue to rotate the camera */
	}
	glutPostRedisplay();
}
/*
* reshape
*
* Window reshape callback.
*
*/

/*
* menuSelect
*
* Menu selection callback.
*
*/
void menuSelect(int value)
{
	switch (value)
	{
	case PAUSE:
		wantPause = 1 - wantPause;
		break;
	case NORMALIZE_SPEED:
		wantNormalize = 1 - wantNormalize;
		break;
	case QUIT:
		exit(0);
		break;
	}
}
/*
* main
*
* Setup OpenGL and hand over to GLUT.
*
*/
int main(int argc,
	char *argv[])
{
	SetupPyramid(1.5);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutCreateWindow("Explosion demo");
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	srand(time(NULL));
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0Amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0Spec);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1Amb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1Spec);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
	glEnable(GL_NORMALIZE);
	glutCreateMenu(menuSelect);
	glutAddMenuEntry("Pause", PAUSE);
	glutAddMenuEntry("Toggle normalized speed vectors", NORMALIZE_SPEED);
	glutAddMenuEntry("Quit", QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
	return 0;
}


#pragma warning(pop)