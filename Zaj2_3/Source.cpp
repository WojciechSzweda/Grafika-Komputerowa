#include "gltools_extracted.h"
#include "VectorMath.h"

// Wielkoci obrot�w
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

constexpr int verticesCount = 7;
constexpr float radius = 1.0f;
// Zmiana przestrzeni widocznej i okna.
// Wywo�ywana w momencie zmiany rozmiaru okna
void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;
	// Zr�wnanie wielko�ci widoku i okna
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	// Ustalenie uk�adu wsp�rz�dnych
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Utworzenie rzutowania perspektywicznego
	gluPerspective(35.0f, fAspect, 1.0, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Ta funkcja wykonuje wszystkie konieczne inicjalizacje kontekstu renderowania.
// Tutaj, konfiguruje i inicjalizuje o�wietlenie sceny
void SetupRC()
{

	GLuint texture;
	int x, y;
	char pixels[256 * 256];
	GLint iWidth = 256;
	GLint iHeight = 256;
	GLint iComponents = GL_RGB;
	GLenum eFormat = {
		GL_RGBA
	};
	// Wartoci i wsp�rzdne �wiat�a
	GLfloat whiteLight[] = { 0.05f, 0.05f, 0.05f, 1.0f };
	GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	GLfloat lightPos[] = { -10.f, 5.0f, 5.0f, 1.0f };
	glEnable(GL_DEPTH_TEST); // Usuwanie ukrytych powierzchni
	glEnable(GL_CULL_FACE); // Nie bdziemy prowadzi oblicze wntrza samolotu
	glFrontFace(GL_CCW); // Wielokty z nawiniciem przeciwnym do ruchu wskaz�wek zegara
						 // W�czenie owietlenia
	glEnable(GL_LIGHTING);
	// Konfiguracja i w��czenie �wiat�a numer 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	// W�czenie ledzenia kolor�w
	glEnable(GL_COLOR_MATERIAL);
	// W�aciwoci owietlenia otoczenia i rozproszenia
	// bd ledzi wartoci podawane funkcji glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// Czarne t�o
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// �adowanie tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate a name for the texture
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);


	for (y = 0; y < 256; y++)
	{
		for (x = 0; x < 256; x++)
			pixels[y * 256 + x] = rand() % 256;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 256, 256, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

// Reakcje na klawisze strza�ek
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
	xRot = (GLfloat)((const int)xRot % 360);
	yRot = (GLfloat)((const int)yRot % 360);
	// Odwieenie zawartoci okna
	glutPostRedisplay();
}
// Wywo�ywana w celu przerysowania sceny
void RenderScene(void)
{
	GLTVector3 vNormal;
	GLTVector3 center[1] = { 0,0,0 };
	GLTVector3 vCorners[verticesCount + 1];

	//G�ra 0
	vCorners[0][0] = 0.0f;
	vCorners[0][1] = 1.2f;
	vCorners[0][2] = 0.0f;
	GLdouble degree = 2 * M_PI / verticesCount;
	for (int i = 1; i < verticesCount + 1; i++)
	{
		vCorners[i][0] = radius*cos(i*degree);
		vCorners[i][1] = 0;
		vCorners[i][2] = radius*sin(i*degree);
	}
	// Czyszczenie okna aktualnym kolorem czyszcz�cym
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Zapisanie stanu macierzy i wykonanie obrot�w
	glPushMatrix();
	// Cofni�cie obiekt�w
	glTranslatef(0.0f, -0.25f, -4.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	// Rysowanie piramidy
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);
	//glNormal3f(0.0f, -1.0f, 0.0f);

	// Podstawa piramidy
	for (int i = 1; i <= verticesCount; i++)
	{
		int j = i + 1;
		if (i == verticesCount)
			j = 1;
		gltGetNormalVector(vCorners[0], vCorners[j], vCorners[i], vNormal);
		glNormal3fv(vNormal);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(center[0]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(vCorners[i]);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3fv(vCorners[j]);
	}

	//Boki piramidy
	for (int i = 1; i <= verticesCount; i++)
	{
		int j = i + 1;
		if (i == verticesCount)
			j = 1;

		gltGetNormalVector(vCorners[0], vCorners[j], vCorners[i], vNormal);
		glNormal3fv(vNormal);
		glTexCoord2f(0.5f, 1.0f);
		glVertex3fv(vCorners[0]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(vCorners[j]);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3fv(vCorners[i]);
	}
	glEnd();
	
	// Odtworzenie stanu macierzy
	glPopMatrix();
	// Zamiana bufor�w
	glutSwapBuffers();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Textured Pyramid - 7k�t");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	return 0;
}
