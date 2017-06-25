#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "glext.h"
#include "Textures.h"


// stałe do obsługi menu podręcznego

enum
{
	FULL_WINDOW,    // aspekt obrazu - całe okno
	ASPECT_1_1,     // aspekt obrazu 1:1
	EXIT            // wyjście
};

// aspekt obrazu

int aspect = FULL_WINDOW;

// usunięcie definicji makr near i far

#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

// rozmiary bryły obcinania

const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

// współczynnik skalowania

GLfloat scale = 0.5;

// identyfikatory list wyświetlania

GLint RECT_LIST;
GLint SPECTRUM_1_LIST, SPECTRUM_2_LIST, SPECTRUM_3_LIST;

// filtr pomniejszający

GLint min_filter = GL_NEAREST_MIPMAP_NEAREST;

// funkcja generująca scenę 3D

void DisplayScene()
{
	// kolor tła - zawartośæ bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	// przesunięcie układu współrzędnych obiektów do środka bryły odcinania
	glTranslatef(0, 0, -(near + far) / 2);

	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, 1.0, 1.0);

	// włączenie teksturowania jednowymiarowego
	glEnable(GL_TEXTURE_1D);

	// tryb upakowania bajtów danych tekstury
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// filtr powiększający
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// usunięcie błędów przy renderingu brzegu tekstury
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// filtr pomniejszający
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, min_filter);

	// wyświetlenie tekstur
	glCallList(SPECTRUM_1_LIST);
	glCallList(SPECTRUM_2_LIST);
	glCallList(SPECTRUM_3_LIST);

	// skierowanie poleceñ do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();
}

// zmiana wielkości okna

void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokośæ okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else

			// szerokośæ okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);

	// generowanie sceny 3D
	DisplayScene();
}

// obsługa klawiatury

void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.05;
	else

		// klawisz -
		if (key == '-' && scale > 0.05)
			scale -= 0.05;

	// narysowanie sceny
	DisplayScene();
}

// obsługa menu podręcznego

void Menu(int value)
{
	switch (value)
	{
		// filtr pomniejszający
	case GL_NEAREST_MIPMAP_NEAREST:
	case GL_NEAREST_MIPMAP_LINEAR:
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		min_filter = value;
		DisplayScene();
		break;

		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		// wyjście
	case EXIT:
		exit(0);
	}
}

// utworzenie list wyświetlania

void GenerateDisplayLists()
{
	// generowanie identyfikatora listy wyświetlania
	RECT_LIST = glGenLists(1);

	// lista wyświetlania - prostokąt
	glNewList(RECT_LIST, GL_COMPILE);

	// nałżenie tekstury na prostokąt
	glBegin(GL_QUADS);
	glTexCoord1f(1.0);
	glVertex2f(1.5, 0.7);
	glTexCoord1f(0.0);
	glVertex2f(-1.5, 0.7);
	glTexCoord1f(0.0);
	glVertex2f(-1.5, -0.7);
	glTexCoord1f(1.0);
	glVertex2f(1.5, -0.7);
	glEnd();

	// koniec listy wyświetlania
	glEndList();

	// sprawdzenie czy implementacja biblioteki obsługuje tekstury o wymiarze 256
	GLint size;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &size);
	if (size < 512)
	{
		printf("Rozmiar tekstur mniejszy od 512");
		exit(0);
	}

	// generowanie identyfikatora listy wyświetlania
	SPECTRUM_1_LIST = glGenLists(1);

	// lista wyświetlania - pierwsza "tęcza"
	glNewList(SPECTRUM_1_LIST, GL_COMPILE);

	// definiowanie tekstury i mipmap
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_512);
	glTexImage1D(GL_TEXTURE_1D, 1, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_256);
	glTexImage1D(GL_TEXTURE_1D, 2, GL_RGB, 128, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_128);
	glTexImage1D(GL_TEXTURE_1D, 3, GL_RGB, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_64);
	glTexImage1D(GL_TEXTURE_1D, 4, GL_RGB, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_32);
	glTexImage1D(GL_TEXTURE_1D, 5, GL_RGB, 16, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_16);
	glTexImage1D(GL_TEXTURE_1D, 6, GL_RGB, 8, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_8);
	glTexImage1D(GL_TEXTURE_1D, 7, GL_RGB, 4, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_4);
	glTexImage1D(GL_TEXTURE_1D, 8, GL_RGB, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_2);
	glTexImage1D(GL_TEXTURE_1D, 9, GL_RGB, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_1);

	// odłżenie macierzy modelowania na stos
	glPushMatrix();

	// przesunięcie prostokąta do góry o dwie jednostki
	glTranslatef(0.0, 2.0, 0.0);

	// nałżenie tekstury na prostokąt
	glCallList(RECT_LIST);

	// zdjęcie macierzy modelowania ze stosu
	glPopMatrix();

	// koniec listy wyświetlania
	glEndList();

	// generowanie identyfikatora listy wyświetlania
	SPECTRUM_2_LIST = glGenLists(1);

	// lista wyświetlania - druga "tęcza"
	glNewList(SPECTRUM_2_LIST, GL_COMPILE);

	// definiowanie tekstury łącznie z mipmapami
	gluBuild1DMipmaps(GL_TEXTURE_1D, GL_RGB, 512, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_512);

	// nałżenie tekstury na prostokąt
	glCallList(RECT_LIST);

	// koniec listy wyświetlania
	glEndList();

	// generowanie identyfikatora listy wyświetlania
	SPECTRUM_3_LIST = glGenLists(1);

	// lista wyświetlania - trzecia "tęcza"
	glNewList(SPECTRUM_3_LIST, GL_COMPILE);

	// włączenie automatycznego generowania mipmap
	glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP, GL_TRUE);

	// definiowanie tekstury
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, Textures::spectrum_512);

	// wyłączenie automatycznego generowania mipmap
	glTexParameteri(GL_TEXTURE_1D, GL_GENERATE_MIPMAP, GL_FALSE);

	// odłżenie macierzy modelowania na stos
	glPushMatrix();

	// przesunięcie prostokąta do dołu o dwie jednostki
	glTranslatef(0.0, -2.0, 0.0);

	// nałżenie tekstury na prostokąt
	glCallList(RECT_LIST);

	// zdjęcie macierzy modelowania ze stosu
	glPopMatrix();

	// koniec listy wyświetlania
	glEndList();
}

void Mouse(int key, int state, int x, int y) {
	//std::cout << key << std::endl;
	if (key == 3)
	{
		scale += 0.05;
	}
	else if (key == 4 && scale > 0.05)
	{
		scale -= 0.05;
	}


	DisplayScene();
}

// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ

void ExtensionSetup()
{
	// pobranie numeru wersji biblioteki OpenGL
	const char *version = (char*)glGetString(GL_VERSION);

	// odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
#ifdef WIN32
		printf("Błędny format wersji OpenGL\n");
#else

		printf("Bledny format wersji OpenGL\n");
#endif

		exit(0);
	}

	// sprawdzenie czy jest co najmniej wersja 1.4 OpenGL lub
	// czy jest obsługiwane rozszerzenie GL_SGIS_generate_mipmap
	if (!(major > 1 || minor >= 4) &&
		!glutExtensionSupported("GL_SGIS_generate_mipmap"))
	{
		printf("Brak rozszerzenia GL_SGIS_generate_mipmap!\n");
		exit(0);
	}

	// sprawdzenie czy jest co najmniej wersja 1.2 OpenGL lub
	// czy jest obsługiwane rozszerzenie GL_SGIS_texture_edge_clamp
	if (!(major > 1 || minor >= 2) &&
		!glutExtensionSupported("GL_SGIS_texture_edge_clamp"))
	{
		printf("Brak rozszerzenia GL_SGIS_texture_edge_clamp!\n");
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// rozmiary głównego okna programu
	glutInitWindowSize(500, 500);

	// utworzenie głównego okna programu
	glutCreateWindow("Mipmapy 1D");

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(DisplayScene);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);


	// utworzenie podmenu - Filtr pomniejszający
	int MenuMinFilter = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST);
	glutAddMenuEntry("GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR);

	// utworzenie podmenu - Aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32

	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
#else

	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif

	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);

	// menu główne
	glutCreateMenu(Menu);

#ifdef WIN32

	glutAddSubMenu("Filtr pomniejszający", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjście", EXIT);
#else

	glutAddSubMenu("Filtr pomniejszajacy", MenuMinFilter);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
	glutAddMenuEntry("Wyjscie", EXIT);
#endif

	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// sprawdzenie i przygotowanie obsługi wybranych rozszerzeñ
	ExtensionSetup();

	// utworzenie list wyświetlania
	GenerateDisplayLists();

	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}