#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include <mmsystem.h>
#include "star_data.h"
#include "image_loader.h"
#include <windows.h>
#include <shellapi.h>

using namespace std;

// Tamaño de la ventana
GLint width = 800;
GLint height = 600;

//Luz
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };  /* Red diffuse light. */
GLfloat light_position[] = { 0.5, 0.0, 0.0, 1.0 };  /* Infinite light location. */

//Menu
static int window;
static int idMenu;
static int idSubMenuVerInfo;
int optionPrimitive = 0;
int totalFiguras = 0;

// Parámetros de la cámara virtual
GLint mouseButton = 0;
GLboolean mouseRotate = false;
GLboolean mouseZoom = false;
GLboolean mousePan = false;
GLboolean dibujarSistema = true;
GLint xClick = 0;
GLint yClick = 0;
GLint xAux = 0;
GLint yAux = 0;
GLfloat rotX = 0.f;
GLfloat rotY = 0.f;
GLfloat panX = 0.f;
GLfloat panY = 0.f;
GLfloat zoomZ = -10.f;
GLfloat rot = 1.f;
GLfloat deltaRot = 0.01f;

//Texturas
GLUquadric *quad;
GLuint textures[10];

void makeTexture(char *image_name, GLuint textureId) {
	Image *image = loadBMP(image_name);
	//GLuint textureId;
	//glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	delete image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int listnum = 1;
float wavepos = 0.0f;

void menuChooser(int choose){
	switch (choose){
	case 0:
		glutDestroyWindow(window);
		break;
	case 1:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/mercurio.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 2:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/venus.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 3:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/tierra.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 4:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/marte.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 5:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/jupiter.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 6:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/saturno.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 7:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/urano.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 8:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/neptuno.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	case 9:
		ShellExecuteA(GetDesktopWindow(), ("open"), LPCSTR("http://www.astromia.com/solar/pluton.htm"), NULL, NULL, SW_SHOWNORMAL);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void createMenu(void){

	//Crea el submenu
	idSubMenuVerInfo = glutCreateMenu(menuChooser);

	//Añade el menu de las figuras Wire
	glutAddMenuEntry("Mercurio", 1);
	glutAddMenuEntry("Venus", 2);
	glutAddMenuEntry("Tierra", 3);
	glutAddMenuEntry("Marte", 4);
	glutAddMenuEntry("Jupiter", 5);
	glutAddMenuEntry("Saturno", 6);
	glutAddMenuEntry("Urano", 7);
	glutAddMenuEntry("Neptuno", 8);
	glutAddMenuEntry("Pluton", 9);

	//Crea el menu principal
	idMenu = glutCreateMenu(menuChooser);

	//Añade los submenus al principal
	glutAddSubMenu("Ver Información sobre Planetas", idSubMenuVerInfo);
	//glutAddMenuEntry("Clear", 100); //Para limpiar la pantalla 
	glutAddMenuEntry("Quit", 0); //Para salir
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

}

// Función que inicializa el Contexto OpenGL y la geometría de la escena
void init() {
	zoomZ = ((10 - xClick) / 10.0) - 1000;
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glColor3f(1.f, 1.f, 1.f);
	/* Enable a single OpenGL light. */
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	quad = gluNewQuadric();
	glGenTextures(10, textures);
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluQuadricTexture(quad, TRUE);
	gluQuadricNormals(quad, GLU_SMOOTH);
	makeTexture("sol.bmp", textures[0]);
	makeTexture("mercurio.bmp", textures[1]);
	makeTexture("venus.bmp", textures[2]);
	makeTexture("tierra.bmp", textures[3]);
	makeTexture("marte.bmp", textures[4]);
	makeTexture("jupiter.bmp", textures[5]);
	makeTexture("saturno.bmp", textures[6]);
	makeTexture("urano2.bmp", textures[7]);
	makeTexture("neptuno.bmp", textures[8]);
	makeTexture("pluton.bmp", textures[9]);

}

// Función que se invoca cada vez que se redimensiona la ventana
void resize(GLint w, GLint h) {
	if (h == 0) {
		h = 1;
	}
	width = w;
	height = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(25.f, (float)width / height, 0.1f, 10000.f);		// Transf. de Proyección en Perspectiva
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);								// Transf. de Viewport (mapeo al área de dibujo)
}


void geomScenaSistemaSolar() {
	glPushMatrix();
	//Sol
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glEnable(GL_TEXTURE_2D);
	gluSphere(quad, 109.f, 20, 20);
	glDisable(GL_TEXTURE_2D);
	//glRotated(45, 0, 0, 0);
	//glutWireSphere(109, 20, 20);
	glPushMatrix();

	//Mercurio
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*4.14, 0.f, 1.f, 0.f);
	glTranslatef(167.f, 0.f, 0.f);
	glRotatef(rot*.017, 0.f, 1.f, 0.f);
	//glutWireSphere(.38f, 20, 20);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	gluSphere(quad, 3.8f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Venus
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*1.62, 0.f, 1.f, 0.f);
	glTranslatef(217.f, 0.f, 0.f);
	glRotatef(rot*.004, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(.95f, 20, 20);
	gluSphere(quad, 9.5f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Tierra
	glBindTexture(GL_TEXTURE_2D, textures[3]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot, 0.f, 1.f, 0.f);
	glTranslatef(259.f, 0.f, 0.f);
	glRotatef(rot, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	gluSphere(quad, 10.f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Marte
	glBindTexture(GL_TEXTURE_2D, textures[4]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*0.53, 0.f, 1.f, 0.f);
	glTranslatef(337.f, 0.f, 0.f);
	glRotatef(rot, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(.53f, 20, 20);
	gluSphere(quad, 5.3f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Jupiter
	glBindTexture(GL_TEXTURE_2D, textures[5]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*.084, 0.f, 1.f, 0.f);
	glTranslatef(887.f, 0.f, 0.f);
	glRotatef(rot*2.4, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(11.19f, 20, 20);
	gluSphere(quad, 55.95f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Saturno
	glBindTexture(GL_TEXTURE_2D, textures[6]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*.034, 0.f, 1.f, 0.f);
	glTranslatef(1538.f, 0.f, 0.f);
	glRotatef(rot*2.18, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(9.40f, 20, 20);
	gluSphere(quad, 42.f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	//Anillo
	glBindTexture(GL_TEXTURE_2D, textures[7]);
	glEnable(GL_TEXTURE_2D);
	glRotated(-45.0, 1.0, 0.0, 0.0);
	gluDisk(quad, 42.6f, 53.f, 20, 20);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPushMatrix();

	//Urano
	glBindTexture(GL_TEXTURE_2D, textures[7]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*.012, 0.f, 1.f, 0.f);
	glTranslatef(2979.f, 0.f, 0.f);
	glRotatef(rot*1.41, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(4.04f, 20, 20);
	gluSphere(quad, 40.4f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Neptuno 
	glBindTexture(GL_TEXTURE_2D, textures[8]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*.00606, 0.f, 1.f, 0.f);
	glTranslatef(5600.f, 0.f, 0.f);
	glRotatef(rot*1.5, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(3.88f, 20, 20);
	gluSphere(quad, 38.8f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPushMatrix();

	//Pluton
	glBindTexture(GL_TEXTURE_2D, textures[9]);
	glEnable(GL_TEXTURE_2D);
	glRotatef(rot*.004, 0.f, 1.f, 0.f);
	glTranslatef(6009.f, 0.f, 0.f);
	glRotatef(rot*.16, 0.f, 1.f, 0.f);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(.18f, 20, 20);
	gluSphere(quad, 1.8f, 20, 20);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPopMatrix();

}

// Función que se invoca cada vez que se dibuja
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//Se limpian los buffers con el color activo definido por glClearColor

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Transformación de vista
	glTranslatef(panX, panY, zoomZ);
	glRotatef(rotY, 1.0, 0.0, 0.0);
	glRotatef(rotX, 0.0, 1.0, 0.0);

	// Escena
	geomScenaSistemaSolar();

	glutSwapBuffers();			// Se intercambian buffers
}

void mouse(int button, int state, int x, int y) {
	mouseButton = button;
	mouseRotate = false;
	mouseZoom = false;
	mousePan = false;
	xClick = x;
	yClick = y;
	if (button == GLUT_LEFT_BUTTON) {
		mouseRotate = true;
		xAux = rotX;
		yAux = rotY;
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		mouseZoom = true;
		xAux = zoomZ;
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		mousePan = true;
		xAux = panX;
		yAux = panY;
	}
}

void mouseMotion(int x, int y) {
	if (mouseRotate == true) {
		if (mouseButton == GLUT_LEFT_BUTTON) {
			if ((x - xClick + xAux) > 360.0) {
				xAux = xAux - 360.0;
			}
			if ((x - xClick + xAux) < 0.0) {
				xAux = xAux + 360.0;
			}
			if ((y - yClick + yAux) > 360.0) {
				yAux = yAux - 360.0;
			}
			if ((y - yClick + yAux) < 0.0) {
				yAux = yAux + 360.0;
			}
			rotX = x - xClick + xAux;
			rotY = y - yClick + yAux;
		}
	}
	else if (mouseZoom == true) {
		if (mouseButton == GLUT_RIGHT_BUTTON) {
			zoomZ = ((x - xClick) / 10.0) + xAux;
		}
	}
	else if (mousePan == true) {
		if (mouseButton == GLUT_MIDDLE_BUTTON) {
			panX = ((x - xClick) / 63.0) + xAux;
			panY = ((y - yClick) / (-63.0)) + yAux;
		}
	}
	glutPostRedisplay();
}

void idle() {
	rot = rot + deltaRot;
	glutPostRedisplay();
}

int main(GLint argc, GLchar **argv) {
	PlaySound(TEXT("SpaceMountain.wav"), NULL, SND_LOOP | SND_ASYNC);
	
	// 1. Se crea una ventana y un contexto OpenGL usando GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Proyecto Final: Sistema Solar");

	// 1.2 Se definen las funciones callback para el manejo de eventos
	glutReshapeFunc(resize);
	glutDisplayFunc(render);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutIdleFunc(idle);

	// 2. Se direcciona a las funciones correctas del API de OpenGL
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error");
		return false;
	}

	// 3. Se inicializa el contexto de OpenGL y la geometría de la escena
	init();
	createMenu();
	//glutKeyboardFunc(keyboard);

	// 4. Se inicia el ciclo de escucha de eventos
	glutMainLoop();
	return 0;
}