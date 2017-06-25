
#ifdef __APPLE__

#include <GLUT/glut.h>

#else

#include <GL/glut.h>

#endif


#include <math.h>
// Definiujemy sta³ o wartoci liczby PI

#define GL_PI 3.14159265f

// Wielko obrotów

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;


// Wywo³ywana w celu przerysowania sceny

void RenderScene(void)
{

	GLfloat sizes[2]; // Do zapisania dopuszczalnego zakresu wielkoœci
	GLfloat step; // Zapamiêtuje obs³ugiwany przyrost rozmiaru
	GLfloat curSize; // Zapamêituje aktualny rozmiar
	GLfloat x, y, z, angle; // Zmienne przechowujce wspó³rzdne i kty

	// Wyczyszczenie okna aktualnym kolorem czyszczcym
	glClear(GL_COLOR_BUFFER_BIT);

	// Zapisanie stanu macierzy i wykonanie obrotu
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	// Pobranie dopuszczalnych rozmiarów punktów
	glGetFloatv(GL_POINT_SIZE_RANGE, sizes);
	glGetFloatv(GL_POINT_SIZE_GRANULARITY, &step);
	step /= 10;
	// Ustalenie pocz¹tkowego rozmiaru punktu
	curSize = sizes[0];

	// Wywo³a tylko raz, przed rysowaniem wszystkich punktów
	x = -160.0f;
	for (angle = 0.0f; angle <= (2.0f*GL_PI)*10.0f; angle += 0.1f)
	{
		z = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		// Okrelenie punktu i przesunicie wspó³rzdnej Z

		glPointSize(curSize);
		glBegin(GL_POINTS);

		glVertex3f(x, y, z);
		glEnd();
		x += 0.5f;
		curSize += step;
	}

	// Zakoczenie rysowania punktów


	// Odtworzenie macierzy przekszta³ce

	glPopMatrix();

	// Wykonanie polece rysowania

	glutSwapBuffers();

}

// Ta funkcja wykonuje wszystkie konieczne inicjalizacje kontekstu renderowania

void SetupRC()

{

	// Czarne t³o

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Bdziemy rysowa kolorem zielonym

	glColor3f(0.0f, 1.0f, 0.0f);

}

void SpecialKeys(int key, int x, int y)

{

	if (key == GLUT_KEY_UP)

		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)

		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)

		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)

		yRot += 5.0f;

	if (key > 356.0f)

		xRot = 0.0f;

	if (key < -1.0f)

		xRot = 355.0f;

	if (key > 356.0f)

		yRot = 0.0f;

	if (key < -1.0f)

		yRot = 355.0f;

	// Odwieenie zawartoci okna

	glutPostRedisplay();

}

void ChangeSize(int w, int h)

{

	GLfloat nRange = 100.0f;

	// Zabezpieczenie przed dzieleniem przez zero

	if (h == 0)

		h = 1;

	// Ustalenie wymiarów widoku na zgodnych z wymiarami okna

	glViewport(0, 0, w, h);

	// Ponowne ustawienie stosu macierzy rzutowania

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	// Utworzenie przestrzeni ograniczajcej (lewo, prawo, dó³, góra, blisko, daleko)

	if (w <= h)

		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);

	else

		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);

	// Ponowne ustawienie stosu macierzy rzutowania

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

}

int main(int argc, char* argv[])

{

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000, 600);

	glutCreateWindow("Program 1 - 10 obrotów - x");

	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);

	glutDisplayFunc(RenderScene);

	SetupRC();

	glutMainLoop();

	return 0;

}

