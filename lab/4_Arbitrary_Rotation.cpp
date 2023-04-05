/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
/***freeglut***/
#include <.\GL\freeglut.h>
/***C/C++  http://bbs.csdn.net/topics/100091584***/
#include <stdio.h> 
#include <stdlib.h>
/***freeglut***/
#include <math.h>
#include <iostream>
void MatrixReset(void);
void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z);
void ChangeSize(int, int);
void RenderScene(void);
void mouseclick(int, int, int, int);
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
int tx = 0, ty = 0, tz = 0;
int Sx = 1, Sy = 1, Sz = 1;
GLdouble posX = 0, posY = 0, posZ = 0;
float thetax = 0, thetay = 0, thetaz = 0;
bool status = false;
bool clickstatus = false;
#define PI 3.14159
GLfloat rotMatrix[] = {
1.0, 0.0, 0.0, 0.0,
0.0, 1.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
0.0, 0.0, 0.0, 1.0
};
int main(int argc, char** argv)
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();

	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(mouseclick);
	glutSpecialFunc(mySpecialKey);
	glutMainLoop();
	return 0;
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
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'r':
		tx = 0; ty = 0; tz = 0;
		thetax = 0; thetay = 0; thetaz = 0;
		break;
	case 'a':
		thetax += 3;
		status = true;
		break;
	case 'd':
		thetax -= 3;
		status = true;
		break;
	case 'w':
		thetay += 3;
		break;
	case 's':
		thetay -= 3;
		break;
	case 'z':
		thetaz += 3;
		break;
	case 'x':
		thetaz -= 3;
		break;
	case 't':
		tz += 3;
		break;
	case 'y':
		tz -= 3;
		break;

	case 'u':
		Sx += 3;
		break;
	case 'i':
		Sx -= 3;
		break;
	case 'o':
		Sy += 3;
		break;
	case 'p':
		Sy -= 3;
		break;
	case 'j':
		Sz += 3;
		break;
	case 'k':
		Sz -= 3;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void mySpecialKey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		tx -= 5;
		break;
	case GLUT_KEY_RIGHT:
		tx += 5;
		break;
	case GLUT_KEY_UP:
		ty += 5;
		break;
	case GLUT_KEY_DOWN:
		ty -= 5;
		break;
	default:
		break;
	}
	glutPostRedisplay();
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
	//畫線
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	glVertex3f(100.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glEnd();

	//繞任意軸旋轉	
	if (status == true) {
		RotateMatrix(thetax, posX, posY, 0);
		glMultMatrixf(rotMatrix);
	}
	//畫立方體
	glColor3f(1, 1, 0);
	glutSolidCube(6);
	//
	if (status == true) {
		RotateMatrix(-thetax, posX, posY, 0);
		glMultMatrixf(rotMatrix);
		status = false;
	}

	if (clickstatus == true) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(2.0f);

		glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(posX, posY, 0.0f);
		glEnd();
	}
	glColor3f(1, 0, 0);
	glPointSize(6);
	

	glBegin(GL_POINTS);
	glVertex3f(posX, posY, 0);
	glEnd();

	glutSwapBuffers();

}
void MatrixReset() {
	rotMatrix[0] = 1;	rotMatrix[4] = 0; rotMatrix[8] = 0; rotMatrix[12] = 0;

	rotMatrix[1] = 0; 	rotMatrix[5] = 1; rotMatrix[9] = 0; rotMatrix[13] = 0;

	rotMatrix[2] = 0;	rotMatrix[6] = 0; rotMatrix[10] = 1; rotMatrix[14] = 0;

	rotMatrix[3] = 0;	rotMatrix[7] = 0; rotMatrix[11] = 0; rotMatrix[15] = 1;
}
void RotateMatrix(float angle, GLfloat X, GLfloat Y, GLfloat Z) {

	MatrixReset();
	GLfloat Cos = cos(angle * PI / 180);
	GLfloat Sin = sin(angle * PI / 180);
	double magnitude = sqrt(X * X + Y * Y + Z * Z);
	X /= magnitude;
	Y /= magnitude;

	rotMatrix[0] = Cos+(1-Cos)*X*X;	
	rotMatrix[1] = (1-Cos)*Y*X+ Sin * Z ;
	rotMatrix[2] = (1-Cos)*Z*X - Sin * Y;
	rotMatrix[4] = (1-Cos)*X*Y-Sin*Z;
	rotMatrix[5] = Cos+(1-Cos)*Y*Y;
	rotMatrix[6] = (1-Cos)*Z*Y+Sin*X;
	rotMatrix[8] = (1-Cos)*X*Z+Sin*Y;
	rotMatrix[9] = (1-Cos)*Y*Z-Sin*X;
	rotMatrix[10] = Cos+(1-Cos)*Z*Z;
}
void mouseclick(int button, int state, int x, int y) {
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			std::cout << "x" << x << "y" << y << std::endl;
			int width = 400, height = 400;
			double left = -10, right = 10, bottom = -10, top = 10, dnear = -10, dfar = 20;

			//GLfloat winx = 0;
			//GLfloat winy = 0;
			//GLfloat winz = 0;


			//GLint viewport[4];
			//GLdouble modelview[16];
			//GLdouble projection[16];
			//glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			//glGetDoublev(GL_PROJECTION_MATRIX, projection);
			//glGetIntegerv(GL_VIEWPORT, viewport);


			//winx = (float)x;
			//winy = viewport[3] - (float)y;
			//glReadPixels((int)winx, (int)winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);
			//gluUnProject(winx, winy, winz, modelview, projection, viewport, &posX, &posY, &posZ);

			posX = ((x - left ) / width) * (double)20 - (double)10-0.5;
			posY = -((y - top ) / height) * (double)20 + (double)10-0.5;

			clickstatus = true;
			std::cout << "world_x:" << posX << "world_y:" << posY << std::endl;
		}
	default:
		break;
	}
}

