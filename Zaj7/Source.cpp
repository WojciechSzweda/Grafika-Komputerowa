#include <GL/glut.h>
#include <stdlib.h>
#include "colors.h"
#define _USE_MATH_DEFINES
#include <cmath>

// rozmiary bry�y obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near_ = 3.0;
const GLdouble far_ = 7.0;

// sta�e do obs�ugi menu podr�cznego

enum
{
	CUTTING_PLANE, // p�aszczyzna przekroju
	POLYGON_OFFSET, // przesuni�cie warto�ci g��bi
	EXIT // wyj�cie
};

// k�t obrotu kuli

GLfloat angle = 0.0;

// k�ty obrotu sze�cianu

GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

// wska�nik rysowania p�aszczyzna przekroju

bool cutting_plane = true;

// wska�nik przesuni�cia warto�ci g��bi

bool polygon_offset = true;

// wska�nik naci�ni�cia lewego przycisku myszki

int button_state = GLUT_UP;

// po�o�enie kursora myszki

int button_x, button_y;


typedef float Vector3[3];
constexpr int verticesCount = 7;

Vector3 center[1] = { 0,0,0 };
Vector3 vCorners[verticesCount + 1];
void SetupOutPyramid(float radius) {
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




// funkcja generuj�ca scen� 3D

void Display()
{
	// kolor t�a - zawarto�� bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru i bufora g��boko�ci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// wyb�r macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesuni�cie uk�adu wsp�rz�dnych sze�cianu do �rodka bry�y odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);

	// obroty sze�cianu
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);

	// niewielkie powi�kszenie sze�cianu
	glScalef(1.15, 1.15, 1.15);

	// w��czenie testu bufora g��boko�ci
	glEnable(GL_DEPTH_TEST);

	// najpierw rysujemy kul� obracaj�c� si� wewn�trz sze�cianu;
	// z uwagi na celowy brak efekt�w o�wietlenia, obr�t kuli
	// podkre�la druga kula w wersji "szkieletowej"
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


	// rysowanie p�aszczyzny otworu w sze�cianie
	if (cutting_plane)
	{
		// wy��czenie rysowania w buforze kolor�w
		glDrawBuffer(GL_NONE);

		// rysowanie kwadratu cz�ciowo ods�aniaj�cego wn�trze sze�cianu
		// (kwadrat jest po�o�ony o 0,001 jednostki nad bokiem sze�cianu)
		glBegin(GL_QUADS);
		glVertex3f(-0.6, -0.6, 0.51);
		glVertex3f(0.6, -0.6, 0.51);
		glVertex3f(0.6, 0.6, 0.51);
		glVertex3f(-0.6, 0.6, 0.51);
		glEnd();

		// w��czenie rysowania w buforze kolor�w
		glDrawBuffer(GL_BACK);
	}

	// w�a�ciwy sze�cian z obramowaniem, kt�rego rysowanie wymusza brak o�wietlenia
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

	// skierowanie polece� do wykonania
	glFlush();

	// zamiana bufor�w koloru
	glutSwapBuffers();
}

// zmiana wielko�ci okna

void Reshape(int width, int height)
{
	// obszar renderingu - ca�e okno
	glViewport(0, 0, width, height);

	// wyb�r macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bry�y obcinania
	glFrustum(left, right, bottom, top, near_, far_);

	// generowanie sceny 3D
	Display();
}

// obs�uga klawiszy funkcyjnych i klawiszy kursora

void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;

		// kursor w g�r�
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;

		// kursor w d�
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obs�uga przycisk�w myszki

void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapami�tanie stanu lewego przycisku myszki
		button_state = state;

		// zapami�tanie po�o�enia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

// obs�uga ruchu kursora myszki

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

// obs�uga menu podr�cznego

void Menu(int value)
{
	switch (value)
	{
		// p�aszczyzna przekroju
	case CUTTING_PLANE:
		cutting_plane = !cutting_plane;
		Display();
		break;

		// przesuni�cie warto�ci g��bi
	case POLYGON_OFFSET:
		polygon_offset = !polygon_offset;
		Display();
		break;

		// wyj�cie
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

	// rozmiary g��wnego okna programu
	glutInitWindowSize(500, 500);

	// utworzenie g��wnego okna programu
	glutCreateWindow("Z-bufor");

	// do��czenie funkcji generuj�cej scen� 3D
	glutDisplayFunc(Display);

	// do��czenie funkcji wywo�ywanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// do��czenie funkcji obs�ugi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);

	// obs�uga przycisk�w myszki
	glutMouseFunc(MouseButton);

	// obs�uga ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie menu podr�cznego
	glutCreateMenu(Menu);

	// menu g��wne
	glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("P�aszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuni�cie warto�ci g��bi: w��cz/wy��cz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyj�cie", EXIT);
#else

	glutAddMenuEntry("Plaszczyzna przekroju: rysowana/nierysowana", CUTTING_PLANE);
	glutAddMenuEntry("Przesuniecie wartosci glebi: wlacz/wylacz", POLYGON_OFFSET);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// okre�lenie przycisku myszki obs�uguj�cej menu podr�czne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// do��czenie funkcji wywo�ywanej podczas "bezczynno�ci" systemu
	glutIdleFunc(Display);

	// wprowadzenie programu do obs�ugi p�tli komunikat�w
	glutMainLoop();
	return 0;
}
