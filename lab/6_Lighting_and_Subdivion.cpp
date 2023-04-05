#include <stdio.h> 
#include <stdlib.h>
#include <.\GL\freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void buildPopupMenu();
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void setMaterial();
void icoNormVec(int i);
void normalize(float v[3]);
void normCrossProd(float u[3], float v[3], float out[3]);
void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]);
void display();
//�ƹ���m
int outdepth = 2;
float thetax = 0, thetay = 0, thetaz = 0;
bool mode = false;
#define PI 3.14159
#define X .525731112119133606
#define Z .850650808352039932
static GLfloat vdata[12][3] = {
 {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
 {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
 {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};
static GLuint tindices[20][3] = {
 {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
 {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
 {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
 {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
};
GLfloat d1[3], d2[3], n[3];
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecialKey);
	buildPopupMenu();
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}
void menuSelect(int option)
{
	switch (option)
	{
	case 0:
		mode = false;
		glutPostRedisplay();
		break;
	case 1:
		mode = true;
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
	glutAddMenuEntry("line", 0);
	glutAddMenuEntry("fill", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


}
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case '+':
		if (outdepth < 5) {
			outdepth += 1;
		}
		cout << "depth:" << outdepth << endl;
		break;
	case '-':
		if (outdepth > 0) {
			outdepth -= 1;
		}
		cout << "depth:" << outdepth << endl;
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
		thetax -= 3;
		//cout << "thetax:" << thetax << endl;
		break;
	case GLUT_KEY_RIGHT:
		thetax += 3;
		//cout << "thetax:" << thetax << endl;
		break;
	case GLUT_KEY_UP:
		thetay -= 3;
		//cout << "thetay:" << thetay << endl;
		break;
	case GLUT_KEY_DOWN:
		thetay += 3;
		//cout << "thetay:" << thetay << endl;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void SetupRC() {
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	setMaterial();
	glEnable(GL_DEPTH_TEST);

}



void ChangeSize(int w, int h)
{
	printf("Window Size= %d X %d\n", w, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -10, 10, -10, 20);
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

	//
	glTranslatef(-10, 0, 0);
	glRotatef(thetax, 0, 1, 0);
	glRotatef(thetay, 1, 0, 0);
	for (int i = 0; i < 20; i++) {
		
	}
	if (mode == true) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++) {
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
		}
		glEnd();

	}
	else {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++) {
			icoNormVec(i);
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
		}
		glEnd();
	}
	glLoadIdentity();

	glTranslatef(0, 0, 0);
	glRotatef(thetax, 0, 1, 0);
	glRotatef(thetay, 1, 0, 0);
	if (mode == true) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++) {
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
		}
		glEnd();

	}
	else {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++) {
			glNormal3fv(&vdata[tindices[i][0]][0]);
			glVertex3fv(&vdata[tindices[i][0]][0]);
			glNormal3fv(&vdata[tindices[i][1]][0]);
			glVertex3fv(&vdata[tindices[i][1]][0]);
			glNormal3fv(&vdata[tindices[i][2]][0]);
			glVertex3fv(&vdata[tindices[i][2]][0]);
		}
		glEnd();
	}
	glLoadIdentity();

	glTranslatef(+10, 0, 0);
	glRotatef(thetax, 0, 1, 0);
	glRotatef(thetay, 1, 0, 0);
	display();
	glLoadIdentity();
	glutSwapBuffers();

}
void setMaterial() {
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
	GLfloat mat_shininess = { 20.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	glShadeModel(GL_SMOOTH); /*enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0); /* enable light 0 */
}
void subdivide(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3],int depth) {
	GLfloat v12[3], v23[3], v31[3];
	int i;
	if (depth == 0) {
		drawTriangle(v1, v2, v3);
		return;
	}
	for (i = 0; i < 3; i++) {
		v12[i] = (v1[i] + v2[i]) / 2.0;
		v23[i] = (v2[i] + v3[i]) / 2.0;
		v31[i] = (v3[i] + v1[i]) / 2.0;
	}
	normalize(v12);
	normalize(v23);
	normalize(v31);
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}
void display() 
{
	for (int i = 0; i < 20; i++) {
		subdivide(&vdata[tindices[i][0]][0]
			, &vdata[tindices[i][1]][0]
			, &vdata[tindices[i][2]][0]
			, outdepth
		);
	}
}
void drawTriangle(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3]) {
	if (mode == true) {
		glBegin(GL_LINES);
		for (int i = 0; i < 20; i++) {
			glVertex3fv(v1);
			glVertex3fv(v2);
			glVertex3fv(v2);
			glVertex3fv(v3);
			glVertex3fv(v3);
			glVertex3fv(v1);
		}
		glEnd();

	}
	else {
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 20; i++) {
			glNormal3fv(v1);
			glVertex3fv(v1);
			glNormal3fv(v2);
			glVertex3fv(v2);
			glNormal3fv(v3);
			glVertex3fv(v3);
		}
		glEnd();
	}

}
void icoNormVec(int i)
{
	for (int k = 0; k < 3; k++) {
		d1[k] = vdata[tindices[i][0]][k] - vdata[tindices[i][1]][k];
		d2[k] = vdata[tindices[i][1]][k] - vdata[tindices[i][2]][k];
	}
	normCrossProd(d1, d2, n);
	glNormal3fv(n);
}
void normalize(float v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= d; 
	v[1] /= d; 
	v[2] /= d;
}
void normCrossProd(float u[3], float v[3], float out[3])
{
	out[0] = u[1] * v[2] - u[2] * v[1];
	out[1] = u[2] * v[0] - u[0] * v[2];
	out[2] = u[0] * v[1] - u[1] * v[0];
	normalize(out);
}



