#include <stdio.h> 
#include <stdlib.h>
#include <.\GL\freeglut.h>
#include <math.h>
#include <iostream>
void ChangeSize(int, int);
void RenderScene(void);
void mouseclick(int, int, int, int);
void SetupRC();
void buildPopupMenu();
void drawgrid(float);
float posX = 0, posY = 0, posZ = 0;
int status = 0;
bool clickstatus = false;
float drawx = 0, drawy = 0;
void fillgrid(int, float);
#define PI 3.14159
int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();

	buildPopupMenu();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMouseFunc(mouseclick);
	glutMainLoop();
	return 0;
}
void menuSelect(int option)
{
	switch (option)
	{
	case 0:
		clickstatus = false;
		status = 0;
		glutPostRedisplay();
		break;
	case 1:
		clickstatus = false;
		status = 1;
		glutPostRedisplay();
		break;
	case 2:
		clickstatus = false;
		status = 2;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void buildPopupMenu()
{
	int menu_id;
	menu_id = glutCreateMenu(menuSelect);
	glutAddMenuEntry("11", 0);
	glutAddMenuEntry("15", 1);
	glutAddMenuEntry("21", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}
void SetupRC() {
	GLfloat whiteLight[] = { 0.45f,0.45f,0.45f,1.0f };
	GLfloat sourceLight[] = { 0.25f,0.25f,0.25f,1.0f };
	GLfloat lightPos[] = { 0.0f,25.0f,20.0f,0.0f };

	glEnable(GL_LIGHTING);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

}



void ChangeSize(int w, int h)
{
	printf("Window Size= %d X %d\n", w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-10, 10, -10, 10, -10, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void RenderScene(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);

	//畫格子
	glBegin(GL_LINES);
	float step = 0;
	if (status == 0) {
		step = 20.0 / 11.0;
		drawgrid(step);
	}
	else if (status == 1) {
		step = 20.0 / 15.0;
		drawgrid(step);
	}
	else if (status == 2) {
		step = 20.0 / 21.0;
		drawgrid(step);
	}
	glEnd();


	if (clickstatus == true) {
		glColor3f(0.0f, 1.0f, 1.0f);
		glRectf(drawx, drawy, drawx + step, drawy + step);
		glFlush();
	}

	glBegin(GL_POINTS);
	glVertex3f(posX, posY, 0);
	glEnd();

	glutSwapBuffers();

}


void mouseclick(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			int width = 400, height = 400;
			double left = -10, right = 10, bottom = -10, top = 10, dnear = -10, dfar = 20;

			posX = ((x - left) / width) * (double)20 - (double)10 - 0.5;
			posY = -((y - top) / height) * (double)20 + (double)10 - 0.5;

			clickstatus = true;

			if (status == 0) {
				float step = 20.0 / 11.0;
				//判斷x
				int index = -5;
				fillgrid(index, step);
			}
			if (status == 1) {
				float step = 20.0 / 15.0;
				//判斷x
				int index = -7;
				fillgrid(index, step);
			}
			if (status == 2) {
				float step = 20.0 / 21.0;
				//判斷x
				int index = -10;
				fillgrid(index, step);
			}
		}
	default:
		break;
	}
}
void fillgrid(int index, float step)
{
	int x = 0, y = 0;
	for (float i = -10; i <= 10; i += step) {
		//判斷點的位置是否在格子裡
		if ((posX > (i)) && (posX < i + step)) {
			x = index;
			drawx = i;
		}
		if ((posY > (i)) && (posY < i + step)) {
			y = index;
			drawy = i;
		}
		index++;
	}
	std::cout << "(" << x << "," << y << ")" << std::endl;
}
void drawgrid(float step)
{
	for (float i = -10; i <= 10; i += step)
	{
		//畫X的線
		glVertex3f(-10.0f, i, 0.0f);
		glVertex3f(10.0f, i, 0.0f);
		//畫Y的線
		glVertex3f(i, -10.0f, 0.0f);
		glVertex3f(i, 10.0f, 0.0f);
	}
}

