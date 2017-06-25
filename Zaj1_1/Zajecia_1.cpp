#include "stdafx.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include "math.h"

constexpr GLint vertexesCount = 7;
GLfloat vertices[vertexesCount][2];
GLfloat angle = 0.0;
// Pocztkowy rozmiar i pozycja prostokta
GLfloat xCenter = 100.0f;
GLfloat yCenter = 100.0f;
GLsizei rsize = 50;
// Rozmiar kroku (liczba pikseli) w osi x i y
GLfloat xstep = 1.0f;
GLfloat ystep = 1.0f;
// Dane zmieniajcych si� rozmiar�w okna
GLfloat windowWidth;
GLfloat windowHeight;


// Funkcja wywo�ywana w celu narysowania sceny
void RenderScene(void) {
	// Wyczyszczenie okna aktualnym kolorem czyszcz�cym
	glClear(GL_COLOR_BUFFER_BIT);

	// Aktualny kolor rysuj�cy - czerwony
	//    R    G    B
	glColor3f(1.0f, 0.0f, 0.0f);

	// Narysowanie wielok�tu wype�nionego aktualnym kolorem
	glPushMatrix();
	glTranslatef(xCenter, yCenter, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-xCenter, -yCenter, 0);



	glBegin(GL_POLYGON);
	GLdouble degree = 2 * M_PI / vertexesCount;
	for (int i = 0; i < vertexesCount; i++)
	{
		vertices[i][0] = xCenter + rsize*cos(i*degree);
		vertices[i][1] = yCenter + rsize*sin(i*degree);
		glVertex2f(vertices[i][0], vertices[i][1]);
	}
	glEnd();

	glPopMatrix();
	// Wys�anie polece� do wykonania - !!! dla animacji to jest inne polecenie
	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h) {
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;

	// Ustalenie wielko�ci widoku zgodnego z rozmiarami okna
	glViewport(0, 0, w, h);

	// Ustalenie uk�adu wsp�rz�dnych
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Keep the square square, this time, save calculated
	// width and height for later use
	if (w <= h)
	{
		windowHeight = 250.0f*h / w;
		windowWidth = 250.0f;
	}
	else
	{
		windowWidth = 250.0f*w / h;
		windowHeight = 250.0f;
	}

	// Ustanowienie przestrzeni ograniczaj�cej (lewo, prawo, d�, g�ra, blisko, daleko)
	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void TimerFunction(int value) {
	// Odwr�cenie kierunku, je�eli osi�gni�to lew� lub praw� kraw�d�
	if (xCenter > windowWidth - rsize || xCenter < 0)
		xstep = -xstep;

	// Odwr�cenie kierunku, je�eli osi�gni�to doln� lub g�rn� kraw�d�
	if (yCenter > windowHeight - rsize || yCenter < 0)
		ystep = -ystep;
	for (int i = 0; i < vertexesCount; i++)
	{
		if (vertices[i][0] >= windowWidth || vertices[i][0] <= 0)
		{
			xstep = -xstep;
			break;
		}
		if (vertices[i][1] >= windowHeight || vertices[i][1] <= 0)
		{
			ystep = -ystep;
		}
	}
	//Zwiekszanie k�ta rotacji
	angle++;
	if (angle == 360)
	{
		angle = 0;
	}

	// Kontrola obramowania. Wykonywana jest na wypadek, gdyby okno     
	// zmniejszy�o swoj wielko�� w czasie, gdy kwadrat odbija� si� od     
	// kraw�dzi, co mog�oby spowodowa�, �e znalaz� by si� poza      
	// przestrzeni� ograniczajc�.     
	if (xCenter > windowWidth - rsize)
		xCenter = windowWidth - rsize - 1;

	if (yCenter > windowHeight - rsize)
		yCenter = windowHeight - rsize - 1;

	// Wykonanie przesuni�cia kwadratu
	xCenter += xstep;
	yCenter += ystep;

	// Ponowne rysowanie sceny z nowymi wsp�rz�dnymi  
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}


// Ustalenie stanu rendrowania  
void SetupRC(void) {
	glClearColor(0.5f, 0.25f, 0.0f, 1.0f);
}

// G��wny punki wej�cia programu  
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("M�j pierwszy program w GLUT");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}

