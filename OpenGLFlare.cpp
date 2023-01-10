// OpenGLFlare.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <atlimage.h>
#include <Windows.h>
#include "include/GL/freeglut.h"

GLubyte* img;

GLfloat spd = 0.003  /* speed of fireworks */, yPos = 0.0, zPos = 5.0;
GLfloat rad = 0.0;

bool flare = false;

const int fireCount = 5;		// number of fireworks
const int flareCount = 7;		// number of flares per firework

GLfloat xPos[fireCount];


void myinit(void)
{
	glClearColor(0.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);


	glEnable(GL_DEPTH_TEST);

	//	Texture Generation from image file

	GLuint textures;
	glGenTextures(1, &textures);

	int width, height;

	CImage image;
	image.Load(_T("back.jpg"));

	BYTE* bits = (BYTE*)image.GetBits();
	width = image.GetWidth();
	height = image.GetHeight();

	img = new GLubyte[width * height * 3];

	int pitch = image.GetPitch();

	//	Image to pixel data

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			img[width * j * 3 + i * 3] = bits[pitch * (height - j - 1) + i * 3];
			img[width * j * 3 + i * 3 + 1] = bits[pitch * (height - j - 1) + i * 3 + 1];
			img[width * j * 3 + i * 3 + 2] = bits[pitch * (height - j - 1) + i * 3 + 2];
		}	

	glBindTexture(GL_TEXTURE_2D, textures);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_TEXTURE_2D);

	//	Initialize positions of fireworks

	for (int i = 0; i < fireCount; i++)
	{
		xPos[i] = (float)(rand() % 50) / 50 * 5;
	}

	xPos[0] = 2.5;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0, 0, 0, 0, 0, 100, 0, 1, 0);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//	Draw a rectangle with background

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0, 0, 10);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(5, 0, 10);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(5, 5, 10);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(0, 5, 10);
	glEnd();

	//	Now draw fireworks

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

	glColor3f(1.0, 1.0, 0.5);

	if (!flare) {
		for (int i = 0; i < fireCount; i++)
		{
			glPushMatrix();

			glTranslatef(xPos[i], yPos, zPos);
			glScalef(0.035, 0.035, 0.035);

			glutSolidOctahedron();

			glPopMatrix();
		}
	}

	//	Draw flares

	if (flare)
	{
		for (int i = 0; i < fireCount; i++)
		{
			glPushMatrix();
			glTranslatef(xPos[i], yPos, zPos);

			for (int j = 0; j < flareCount; j++)
			{
				glPushMatrix();

				glRotatef(360.0 / flareCount * j, 0, 0, 1);
				glTranslatef(rad, 0, 0);
				glScalef(0.02, 0.02, 0.02);
				glutSolidOctahedron();


				glPopMatrix();
			}
			glPopMatrix();
		} 
	}
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	//	Viewing transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-5, 0, 0, 5, 0, 1000);
}

void idle()
{
	yPos += spd;
	spd -= 0.000001;
	rad += 0.0005;

	if (!flare && spd <= 0)
	{
		flare = true;

		rad = 0;	
	}

	if (flare && rad > 0.5)
	{
		flare = false;

		spd = 0.003;
		yPos = 0;

		//	Relocate fireworks

		for (int i = 0; i < fireCount; i++)
		{
			xPos[i] = (float)(rand() % 50) / 50 * 5;
		}
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Hi, Bezhan");
	myinit();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}