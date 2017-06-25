#include "Program_5.h"

struct Punkt
{
	int X;
	int Y;
	Punkt(int x, int y) {
		X = x;
		Y = y;
	}
};



vector<Punkt> vertices;

// Dane zmieniajcych siê rozmiarów okna
GLfloat windowWidth;
GLfloat windowHeight;

// Funkcja wywo³ywana w celu narysowania sceny
void RenderScene(void) {
	// Wyczyszczenie okna aktualnym kolorem czyszcz¹cym
	glClear(GL_COLOR_BUFFER_BIT);
	// Aktualny kolor rysuj¹cy - czerwony
	//    R    G    B
	glColor3f(1.0f, 0.0f, 0.0f);

	// Narysowanie wielok¹tu wype³nionego aktualnym kolorem
	glBegin(GL_POLYGON);
	for (int i = 0; i < vertices.size(); i++)
	{
		glVertex2f(vertices[i].X, vertices[i].Y);
	}
	glEnd();



	// Wys³anie poleceñ do wykonania - !!! dla animacji to jest inne polecenie
	glutSwapBuffers();
}

void ChangeSize(GLsizei w, GLsizei h) {
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;

	// Ustalenie wielkoœci widoku zgodnego z rozmiarami okna
	glViewport(0, 0, w, h);

	// Ustalenie uk³adu wspó³rzêdnych
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

	// Ustanowienie przestrzeni ograniczaj¹cej (lewo, prawo, dó³, góra, blisko, daleko)
	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void AddVertex(Punkt pnt) {

	vertices.emplace_back(pnt);
	cout << "added " << pnt.X << " " << pnt.Y << endl;

}
Punkt WindowToWorld(int x, int y) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	return Punkt(posX, posY);
}

void MouseDownHandle(int button, int state, int x, int y) {
	cout << "Button: " << button << " state: " << state << " X: " << x << " Y: " << y << endl;
	if (button == 0 && state == 0)
	{

		AddVertex(WindowToWorld(x, y));
	}
	if (button == 2 && state == 0 && vertices.size() > 0)
	{
		vertices.pop_back();
		for (int i = 0; i < vertices.size(); i++)
		{
			cout << vertices[i].X << endl;
		}
	}
}



void TimerFunction(int value) {
	
	glutMouseFunc(MouseDownHandle);

	// Ponowne rysowanie sceny z nowymi wspó³rzêdnymi  
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}


// Ustalenie stanu rendrowania  
void SetupRC(void) {
	glClearColor(0.5f, 0.25f, 0.0f, 1.0f);
}

// G³ówny punki wejœcia programu  
void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Program 5 - Kliknij na okno, aby dodac wierzcholek");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}


