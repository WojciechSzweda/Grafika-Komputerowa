
#include <GL/glut.h>
#include "materials.h"
#include <stdlib.h>


// stałe do obsługi menu podręcznego


enum

{
	BRASS,                // mosiądz
	BRONZE,               // brąz
	POLISHED_BRONZE,      // polerowany brąz
	CHROME,               // chrom
	COPPER,               // miedź
	POLISHED_COPPER,      // polerowana miedź
	GOLD,                 // złoto
	POLISHED_GOLD,        // polerowane złoto
	PEWTER,               // grafit (cyna z ołowiem)
	SILVER,               // srebro
	POLISHED_SILVER,      // polerowane srebro
	EMERALD,              // szmaragd
	JADE,                 // jadeit
	OBSIDIAN,             // obsydian
	PEARL,                // perła
	RUBY,                 // rubin
	TURQUOISE,            // turkus
	BLACK_PLASTIC,        // czarny plastik
	BLACK_RUBBER,         // czarna guma

	FULL_WINDOW,       // aspekt obrazu - całe okno

	ASPECT_1_1,        // aspekt obrazu 1:1

	EXIT               // wyjście

};


// aspekt obrazu


int aspect = FULL_WINDOW;


const GLfloat *ambient = BrassAmbient;
const GLfloat *diffuse = BrassDiffuse;
const GLfloat *specular = BrassSpecular;
GLfloat shininess = BrassShininess;


// rozmiary bryły obcinania


const GLdouble left = -10.0;

const GLdouble right = 10.0;

const GLdouble bottom = -10.0;

const GLdouble top = 10.0;

const GLdouble near_ = 50.0;

const GLdouble far_ = 70.0;


// współczynnik skalowania


GLfloat scale = 1.0;


// kąty obrotu


GLfloat rotatex = 0.0;

GLfloat rotatey = 0.0;


// przesunięcie


GLfloat translatex = 0.0;

GLfloat translatey = 0.0;


// wskaŸnik naciśnięcia lewego przycisku myszki


int button_state = GLUT_UP;


// poło¿enie kursora myszki


int button_x, button_y;


// funkcja rysująca blok 3x3




int RenderTypeIndex = 0;

void WireCube3x3()

{

	glutWireOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutWireOctahedron();

	glTranslatef(0.0, -1.0, 0.0);

	glutWireOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutWireOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutWireOctahedron();

	glTranslatef(0.0, 1.0, 0.0);

	glutWireOctahedron();

	glTranslatef(0.0, 1.0, 0.0);

	glutWireOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutWireOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutWireOctahedron();

}

void SolidCube3x3()

{

	glutSolidOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(0.0, -1.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(0.0, 1.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(0.0, 1.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutSolidOctahedron();

}


// funkcja rysująca blok 2x2


void WireCube2x2()

{
	glutWireOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutWireOctahedron();

	glTranslatef(0.0, -1.0, 0.0);

	glutWireOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutWireOctahedron();

}

void SolidCube2x2()

{
	glutSolidOctahedron();

	glTranslatef(1.0, 0.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(0.0, -1.0, 0.0);

	glutSolidOctahedron();

	glTranslatef(-1.0, 0.0, 0.0);

	glutSolidOctahedron();

}


// funkcja rysująca piramidę z sześcianów

typedef void(*Functions)(void);
typedef void(_stdcall*glutOctohedron)(void);
Functions Cube3x3[] = { WireCube3x3, SolidCube3x3 };
Functions Cube2x2[] = { WireCube2x2, SolidCube2x2 };
glutOctohedron Cube1x1[] = { glutWireOctahedron, glutSolidOctahedron };

void Pyramid()

{

	// podstawa 7x7

	glPushMatrix();
	glTranslatef(2, 2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, 0, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 0, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, -2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2, 2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2, -2, -1);
	Cube3x3[RenderTypeIndex]();
	glPopMatrix();

	// podstawa 6x6

	glPushMatrix();

	glTranslatef(1.5, 1.5, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(1.5, -1.5, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -1.5, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, 1.5, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();


	// drugi poziom 5x5

	glPushMatrix();

	glTranslatef(0.0, 0.0, 1.0);

	glPushMatrix();

	glTranslatef(1.0, 1.0, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(1.0, -1.0, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-2.0, 2.0, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.0, -1.0, 0.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();

	glPopMatrix();


	// trzeci poziom 4x4

	glPushMatrix();

	glTranslatef(0.0, 0.0, 2.0);

	glPushMatrix();

	glTranslatef(0.5, -0.5, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(0.5, 1.5, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, 1.5, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPushMatrix();

	glTranslatef(-1.5, -0.5, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();

	glPopMatrix();


	// czwarty poziom 3x3

	glPushMatrix();

	glTranslatef(0.0, 0.0, 3.0);

	Cube3x3[RenderTypeIndex]();

	glPopMatrix();


	// piąty poziom 2x2

	glPushMatrix();

	glTranslatef(0.0, 0.0, 4.0);

	glTranslatef(-0.5, 0.5, 0.0);

	Cube2x2[RenderTypeIndex]();

	glPopMatrix();


	// szósty poziom 1x1

	glPushMatrix();

	glTranslatef(0.0, 0.0, 5.0);

	Cube1x1[RenderTypeIndex]();

	glPopMatrix();

}


// funkcja generująca scenę 3D


void DisplayScene()

{

	// kolor tła - zawartość bufora koloru

	glClearColor(1.0, 1.0, 1.0, 1.0);


	// czyszczenie bufora koloru

	glClear(GL_COLOR_BUFFER_BIT);


	// wybór macierzy modelowania

	glMatrixMode(GL_MODELVIEW);


	// macierz modelowania = macierz jednostkowa

	glLoadIdentity();


	// włączenie oświetlenia
	glEnable(GL_LIGHTING);

	// włączenie światła GL_LIGHT0
	glEnable(GL_LIGHT0);

	// włączenie automatycznej normalizacji wektorów normalnych
	glEnable(GL_NORMALIZE);

	// przesunięcie układu współrzędnych obiektu do środka bryły odcinania

	glTranslatef(0, 0, -(near_ + far_) / 2);


	// przesunięcie obiektu - ruch myszką

	glTranslatef(translatex, translatey, 0.0);


	// skalowanie obiektu - klawisze "+" i "-"

	glScalef(scale, scale, scale);


	// obroty obiektu - klawisze kursora

	glRotatef(rotatex, 1.0, 0, 0);

	glRotatef(rotatey, 0, 0, 1.0);


	// kolor krawędzi obiektu

	glColor3f(0.0, 0.0, 0.0);


	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);


	// rysowanie piramidy

	Pyramid();


	// skierowanie poleceń do wykonania

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

		// wysokość okna większa od wysokości okna

		if (width < height && width > 0)

			glFrustum(left, right, bottom*height / width, top*height / width, near_, far_);

		else


			// szerokość okna większa lub równa wysokości okna

			if (width >= height && height > 0)

				glFrustum(left*width / height, right*width / height, bottom, top, near_, far_);

	}

	else

		glFrustum(left, right, bottom, top, near_, far_);


	// generowanie sceny 3D

	DisplayScene();

}


// obsługa klawiatury


void Keyboard(unsigned char key, int x, int y)

{

	// klawisz +

	if (key == '+')

		scale += 0.1f;

	else


		// klawisz -

		if (key == '-' && scale > 0.1)

			scale -= 0.1f;


	// odrysowanie okna

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

}


// obsługa klawiszy funkcyjnych i klawiszy kursora


void SpecialKeys(int key, int x, int y)

{

	switch (key)

	{

		// kursor w lewo

	case GLUT_KEY_LEFT:

		translatex -= 1;

		break;


		// kursor w górę

	case GLUT_KEY_UP:

		translatey += 1;

		break;


		// kursor w prawo

	case GLUT_KEY_RIGHT:

		translatex += 1;

		break;


		// kursor w dół

	case GLUT_KEY_DOWN:

		translatey -= 1;

		break;

	}


	// odrysowanie okna

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

}


// obsługa przycisków myszki


void MouseButton(int button, int state, int x, int y)

{

	if (button == GLUT_LEFT_BUTTON)

	{

		// zapamiętanie stanu lewego przycisku myszki

		button_state = state;


		// zapamiętanie poło¿enia kursora myszki

		if (state == GLUT_DOWN)

		{

			button_x = x;

			button_y = y;

		}

	}

}


// obsługa ruchu kursora myszki

GLfloat mouseSensitivity = 4.0f;

void MouseMotion(int x, int y)

{

	if (button_state == GLUT_DOWN)

	{

		rotatey += mouseSensitivity *(GLfloat)(right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);

		button_x = x;

		rotatex += -mouseSensitivity *(GLfloat)(top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);

		button_y = y;

		glutPostRedisplay();

	}

}


// obsługa menu podręcznego


void RenderMenu(int value) {
	RenderTypeIndex = value;
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void MaterialMenu(int value) {
	switch (value)
	{
		// materiał - mosiądz
	case BRASS:
		ambient = BrassAmbient;
		diffuse = BrassDiffuse;
		specular = BrassSpecular;
		shininess = BrassShininess;
		DisplayScene();
		break;

		// materiał - brąz
	case BRONZE:
		ambient = BronzeAmbient;
		diffuse = BronzeDiffuse;
		specular = BronzeSpecular;
		shininess = BronzeShininess;
		DisplayScene();
		break;

		// materiał - polerowany brąz
	case POLISHED_BRONZE:
		ambient = PolishedBronzeAmbient;
		diffuse = PolishedBronzeDiffuse;
		specular = PolishedBronzeSpecular;
		shininess = PolishedBronzeShininess;
		DisplayScene();
		break;

		// materiał - chrom
	case CHROME:
		ambient = ChromeAmbient;
		diffuse = ChromeDiffuse;
		specular = ChromeSpecular;
		shininess = ChromeShininess;
		DisplayScene();
		break;

		// materiał - miedź
	case COPPER:
		ambient = CopperAmbient;
		diffuse = CopperDiffuse;
		specular = CopperSpecular;
		shininess = CopperShininess;
		DisplayScene();
		break;

		// materiał - polerowana miedź
	case POLISHED_COPPER:
		ambient = PolishedCopperAmbient;
		diffuse = PolishedCopperDiffuse;
		specular = PolishedCopperSpecular;
		shininess = PolishedCopperShininess;
		DisplayScene();
		break;

		// materiał - złoto
	case GOLD:
		ambient = GoldAmbient;
		diffuse = GoldDiffuse;
		specular = GoldSpecular;
		shininess = GoldShininess;
		DisplayScene();
		break;

		// materiał - polerowane złoto
	case POLISHED_GOLD:
		ambient = PolishedGoldAmbient;
		diffuse = PolishedGoldDiffuse;
		specular = PolishedGoldSpecular;
		shininess = PolishedGoldShininess;
		DisplayScene();
		break;

		// materiał - grafit (cyna z ołowiem)
	case PEWTER:
		ambient = PewterAmbient;
		diffuse = PewterDiffuse;
		specular = PewterSpecular;
		shininess = PewterShininess;
		DisplayScene();
		break;

		// materiał - srebro
	case SILVER:
		ambient = SilverAmbient;
		diffuse = SilverDiffuse;
		specular = SilverSpecular;
		shininess = SilverShininess;
		DisplayScene();
		break;

		// materiał - polerowane srebro
	case POLISHED_SILVER:
		ambient = PolishedSilverAmbient;
		diffuse = PolishedSilverDiffuse;
		specular = PolishedSilverSpecular;
		shininess = PolishedSilverShininess;
		DisplayScene();
		break;

		// materiał - szmaragd
	case EMERALD:
		ambient = EmeraldAmbient;
		diffuse = EmeraldDiffuse;
		specular = EmeraldSpecular;
		shininess = EmeraldShininess;
		DisplayScene();
		break;

		// materiał - jadeit
	case JADE:
		ambient = JadeAmbient;
		diffuse = JadeDiffuse;
		specular = JadeSpecular;
		shininess = JadeShininess;
		DisplayScene();
		break;

		// materiał - obsydian
	case OBSIDIAN:
		ambient = ObsidianAmbient;
		diffuse = ObsidianDiffuse;
		specular = ObsidianSpecular;
		shininess = ObsidianShininess;
		DisplayScene();
		break;

		// materiał - perła
	case PEARL:
		ambient = PearlAmbient;
		diffuse = PearlDiffuse;
		specular = PearlSpecular;
		shininess = PearlShininess;
		DisplayScene();
		break;

		// metariał - rubin
	case RUBY:
		ambient = RubyAmbient;
		diffuse = RubyDiffuse;
		specular = RubySpecular;
		shininess = RubyShininess;
		DisplayScene();
		break;

		// materiał - turkus
	case TURQUOISE:
		ambient = TurquoiseAmbient;
		diffuse = TurquoiseDiffuse;
		specular = TurquoiseSpecular;
		shininess = TurquoiseShininess;
		DisplayScene();
		break;

		// materiał - czarny plastik
	case BLACK_PLASTIC:
		ambient = BlackPlasticAmbient;
		diffuse = BlackPlasticDiffuse;
		specular = BlackPlasticSpecular;
		shininess = BlackPlasticShininess;
		DisplayScene();
		break;

		// materiał - czarna guma
	case BLACK_RUBBER:
		ambient = BlackRubberAmbient;
		diffuse = BlackRubberDiffuse;
		specular = BlackRubberSpecular;
		shininess = BlackRubberShininess;
		DisplayScene();
		break;
	}

}

void Menu(int value)

{

	switch (value)

	{

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


int main(int argc, char *argv[])

{

	// inicjalizacja biblioteki GLUT

	glutInit(&argc, argv);


	// inicjalizacja bufora ramki

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);


	// rozmiary głównego okna programu

	glutInitWindowSize(400, 400);


	// utworzenie głównego okna programu

	glutCreateWindow("Stos modelowania");


	// dołączenie funkcji generującej scenę 3D

	glutDisplayFunc(DisplayScene);


	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna

	glutReshapeFunc(Reshape);


	// dołączenie funkcji obsługi klawiatury

	glutKeyboardFunc(Keyboard);


	// dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora

	glutSpecialFunc(SpecialKeys);


	// obsługa przycisków myszki

	glutMouseFunc(MouseButton);


	// obsługa ruchu kursora myszki

	glutMotionFunc(MouseMotion);


	// utworzenie podmenu - Materiał
	int MenuMaterial = glutCreateMenu(MaterialMenu);

	glutAddMenuEntry("Mosiądz", BRASS);
	glutAddMenuEntry("Brąz", BRONZE);
	glutAddMenuEntry("Polerowany brąz", POLISHED_BRONZE);
	glutAddMenuEntry("Chrom", CHROME);
	glutAddMenuEntry("Miedź", COPPER);
	glutAddMenuEntry("Polerowana miedź", POLISHED_COPPER);
	glutAddMenuEntry("Złoto", GOLD);
	glutAddMenuEntry("Polerowane złoto", POLISHED_GOLD);
	glutAddMenuEntry("Grafit (cyna z ołowiem)", PEWTER);
	glutAddMenuEntry("Srebro", SILVER);
	glutAddMenuEntry("Polerowane srebro", POLISHED_SILVER);
	glutAddMenuEntry("Szmaragd", EMERALD);
	glutAddMenuEntry("Jadeit", JADE);
	glutAddMenuEntry("Obsydian", OBSIDIAN);
	glutAddMenuEntry("Perła", PEARL);
	glutAddMenuEntry("Rubin", RUBY);
	glutAddMenuEntry("Turkus", TURQUOISE);
	glutAddMenuEntry("Czarny plastik", BLACK_PLASTIC);
	glutAddMenuEntry("Czarna guma", BLACK_RUBBER);



	int MenuRenderType = glutCreateMenu(RenderMenu);

	glutAddMenuEntry("WireFrame Render", 0);
	glutAddMenuEntry("Solid Render", 1);


	// utworzenie podmenu - aspekt obrazu

	int MenuAspect = glutCreateMenu(Menu);

#ifdef WIN32


	glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);

#else


	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);

#endif


	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);


	// menu główne

	glutCreateMenu(Menu);

	
	glutAddSubMenu("Render Type", MenuRenderType);
	glutAddSubMenu("Material", MenuMaterial);
	glutAddSubMenu("Aspekt obrazu", MenuAspect);

#ifdef WIN32


	glutAddMenuEntry("Wyjście", EXIT);

#else


	glutAddMenuEntry("Wyjscie", EXIT);

#endif


	// określenie przycisku myszki obsługującego menu podręczne

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	// wprowadzenie programu do obsługi pętli komunikatów

	glutMainLoop();

	return 0;

}

