#include <GL/glut.h>
#include <stdlib.h>
#include "colors.h"
#define _USE_MATH_DEFINES
#include <cmath>

// rozmiary bry³y obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near_ = 3.0;
const GLdouble far_ = 7.0;

// sta³e do obs³ugi menu podrêcznego

enum
{
	CUTTING_PLANE, // p³aszczyzna przekroju
	POLYGON_OFFSET, // przesuniêcie wartoœci g³êbi
	EXIT // wyjœcie
};

// k¹t obrotu kuli

GLfloat angle = 0.0;

// k¹ty obrotu szeœcianu

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

// wskaŸnik rysowania p³aszczyzna przekroju

bool cutting_plane = true;

// wskaŸnik przesuniêcia wartoœci g³êbi

bool polygon_offset = true;

// wskaŸnik naciœniêcia lewego przycisku myszki

int button_state = GLUT_UP;

// po³o¿enie kursora myszki

int button_x, button_y;


typedef float Vector3[3];
constexpr int verticesCount = 7;

Vector3 center[1] = { 0,0,0 };
Vector3 vCorners[verticesCount + 1];
void SetupOutPyramid(float radius) {
	//Góra 0
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

Vector3 centerIn[1] = { 0,0,0 };
Vector3 vCornersIn[verticesCount + 1];
void SetupInPyramid(float radius) {
	vCornersIn[0][0] = 0.0f;
	vCornersIn[0][1] = radius * 1.5;
	vCornersIn[0][2] = 0.0f;
	GLdouble degree = 2 * M_PI / verticesCount;
	for (int i = 1; i < verticesCount + 1; i++)
	{
		vCornersIn[i][0] = radius*cos(i*degree);
		vCornersIn[i][1] = 0;
		vCornersIn[i][2] = radius*sin(i*degree);
	}
}


void RenderPyramid() {
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0.0f, -0.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3fv(Blue);

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

void RenderInPyramid() {
	glPushMatrix();
	glRotatef(90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0.0f, -0.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3fv(Green);

	//Boki piramidy
	for (int i = 1; i <= verticesCount; i++)
	{
		int j = i + 1;
		if (i == verticesCount)
			j = 1;

		glVertex3fv(vCornersIn[0]);
		glVertex3fv(vCornersIn[j]);
		glVertex3fv(vCornersIn[i]);
	}
	glEnd();
	glPopMatrix();
}




// funkcja generuj¹ca scenê 3D

void Display()
{
	// kolor t³a - zawartoœæ bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru i bufora g³êbokoœci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesuniêcie uk³adu wspó³rzêdnych szeœcianu do œrodka bry³y odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);

	// obroty szeœcianu
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// niewielkie powiêkszenie szeœcianu
	glScalef(1.15, 1.15, 1.15);

	// w³¹czenie testu bufora g³êbokoœci
	glEnable(GL_DEPTH_TEST);

	// najpierw rysujemy kulê obracaj¹c¹ siê wewn¹trz szeœcianu;
	// z uwagi na celowy brak efektów oœwietlenia, obrót kuli
	// podkreœla druga kula w wersji "szkieletowej"
	glPushMatrix();
	angle += 0.2;
	glRotatef(angle, 1.0, 1.0, 0.0);
	glColor3fv(Yellow);
	if (polygon_offset)
		glEnable(GL_POLYGON_OFFSET_FILL);

	glPolygonOffset(1.0, 1.0);
	glutSolidSphere(0.5, 10, 10);
	glColor3fv(Black);
	glutWireSphere(0.5, 10, 10);
	if (polygon_offset)
		glDisable(GL_POLYGON_OFFSET_FILL);

	glPopMatrix();

	RenderInPyramid();


	// rysowanie p³aszczyzny otworu w szeœcianie
	if (cutting_plane)
	{
		// wy³¹czenie rysowania w buforze kolorów
		glDrawBuffer(GL_NONE);

		// rysowanie kwadratu czêœciowo ods³aniaj¹cego wnêtrze szeœcianu
		// (kwadrat jest po³o¿ony o 0,001 jednostki nad bokiem szeœcianu)
		glBegin(GL_QUADS);
		glVertex3f(-0.6, -0.6, 0.51);
		glVertex3f(0.6, -0.6, 0.51);
		glVertex3f(0.6, 0.6, 0.51);
		glVertex3f(-0.6, 0.6, 0.51);
		glEnd();

		// w³¹czenie rysowania w buforze kolorów
		glDrawBuffer(GL_BACK);
	}

	// w³aœciwy szeœcian z obramowaniem, którego rysowanie wymusza brak oœwietlenia
	RenderPyramid();
	glColor3fv(Red);
	if (polygon_offset)
		glEnable(GL_POLYGON_OFFSET_FILL);

	/*glPolygonOffset(1.0, 1.0);
	glutSolidCube(2.0);
	glColor3fv(Black);
	glutWireCube(2.0);*/
	if (polygon_offset)
		glDisable(GL_POLYGON_OFFSET_FILL);

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkoœci okna

void Reshape(int width, int height)
{
	// obszar renderingu - ca³e okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bry³y obcinania
	glFrustum(left, right, bottom, top, near_, far_);

	// generowanie sceny 3D
	Display();
}

// obs³uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

		// kursor w górê
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

		// kursor w dó³
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs³uga przycisków myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiêtanie stanu lewego przycisku myszki
		button_state = state;

		// zapamiêtanie po³o¿enia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obs³uga ruchu kursora myszki

void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) *(x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) *(button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

// obs³uga menu podrêcznego

void Menu(int value)
{
	switch (value)
	{
		// p³aszczyzna przekroju
	case CUTTING_PLANE:
		cutting_plane = !cutting_plane;
		Display();
		break;

		// przesuniêcie wartoœci g³êbi
	case POLYGON_OFFSET:
		polygon_offset = !polygon_offset;
		Display();
		break;

		// wyjœcie
	case EXIT:
		exit(0);
	}
}

int main(int argc, char * argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	SetupOutPyramid(1.5f);
	SetupInPyramid(1.0f);
	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary g³ównego okna programu
	glutInitWindowSize(500, 500);

	// utworzenie g³ównego okna programu
	glutCreateWindow("Z-bufor");

	// do³¹czenie funkcji generuj¹cej scenê 3D
	glutDisplayFunc(Display);

	// do³¹czenie funkcji wywo³ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do³¹czenie funkcji obs³ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// obs³uga przycisków myszki
	glutMouseFunc(MouseButton);

	// obs³uga ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie menu podrêcznego
	glutCreateMenu(Menu);

	// menu g³ówne
	glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("P³aszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuniêcie wartoœci g³êbi: w³¹cz/wy³¹cz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyjœcie", EXIT);
#else

	glutAddMenuEntry("Plaszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuniecie wartosci glebi: wlacz/wylacz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// okreœlenie przycisku myszki obs³uguj¹cej menu podrêczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// do³¹czenie funkcji wywo³ywanej podczas "bezczynnoœci" systemu
	glutIdleFunc(Display);

	// wprowadzenie programu do obs³ugi pêtli komunikatów
	glutMainLoop();
	return 0;
}
