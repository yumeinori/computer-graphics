//#include <stdio.h> 
//#include <stdlib.h>
//#include <.\GL\freeglut.h>
//#include <math.h>
//#include <iostream>
//#include <vector>
//using namespace std;
//
//void ChangeSize(int, int);
//void RenderScene(void);
//void mouseclick(int, int, int, int);
//void SetupRC();
//void buildPopupMenu();
//void drawgrid(float);
//void fillgrid(int, float);
//void midpoint(int, int, int, int, float);
//void setPixel(int x, int y, float step);
//void demoLine(float step);
//void anti_setPixel(int x, int y, float color, float step);
//void anti_aliasing(int x0, int y0, int x1, int y1, float step);
//
////滑鼠位置
//float posX = 0, posY = 0, posZ = 0;
////點擊狀態
//bool clickstatus = false;
////現在使用的演算法
//bool algo_status = false;
//float drawx = 0, drawy = 0;
//#define PI 3.14159
////格子數
//float grid = 11.0;
//
//
//
////輸入的兩點
//int p0_x = 0, p0_y = 0, p1_x = -5, p1_y = -2;
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowSize(400, 400);
//	glutInitWindowPosition(600, 80);
//	glutCreateWindow("Moving Cube");
//
//	SetupRC();
//
//	buildPopupMenu();
//	glutReshapeFunc(ChangeSize);
//	glutDisplayFunc(RenderScene);
//	glutMouseFunc(mouseclick);
//	glutMainLoop();
//	return 0;
//}
//void menuSelect(int option)
//{
//	switch (option)
//	{
//	case 0:
//		clickstatus = false;
//		grid = 11.0;
//		glutPostRedisplay();
//		break;
//	case 1:
//		clickstatus = false;
//		grid = 15.0;
//		glutPostRedisplay();
//		break;
//	case 2:
//		clickstatus = false;
//		grid = 21.0;
//		glutPostRedisplay();
//		break;
//	case 3:
//		algo_status = false;
//		glutPostRedisplay();
//		break;
//	case 4:
//		algo_status = true;
//		glutPostRedisplay();
//		break;
//	default:
//		break;
//	}
//}
//
//
//void buildPopupMenu()
//{
//	int menu_id;
//	menu_id = glutCreateMenu(menuSelect);
//	glutAddMenuEntry("11", 0);
//	glutAddMenuEntry("15", 1);
//	glutAddMenuEntry("21", 2);
//	glutAddMenuEntry("midpoint", 3);
//	glutAddMenuEntry("anti-aliasing", 4);
//	glutAttachMenu(GLUT_RIGHT_BUTTON);
//
//
//}
//void SetupRC() {
//	GLfloat whiteLight[] = { 0.45f,0.45f,0.45f,1.0f };
//	GLfloat sourceLight[] = { 0.25f,0.25f,0.25f,1.0f };
//	GLfloat lightPos[] = { 0.0f,25.0f,20.0f,0.0f };
//
//	glEnable(GL_LIGHTING);
//
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
//	glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
//	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
//	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//	glEnable(GL_LIGHT0);
//	glEnable(GL_COLOR_MATERIAL);
//	glEnable(GL_DEPTH_TEST);
//
//}
//
//
//
//void ChangeSize(int w, int h)
//{
//	printf("Window Size= %d X %d\n", w, h);
//	glViewport(0, 0, w, h);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-10, 10, -10, 10, -10, 20);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}
//void RenderScene(void)
//{
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	glColor3f(1.0f, 0.0f, 0.0f);
//	glLineWidth(1.0f);
//
//	//畫格子
//	glBegin(GL_LINES);
//	float step = 0;
//	step = 20.0 / grid;
//	drawgrid(step);
//
//	glEnd();
//	//畫演示線
//	demoLine(step);
//	////////////////////////////////////////
//	//畫線演算法
//	if (!algo_status) {
//		midpoint(p0_x, p0_y, p1_x, p1_y, step);
//	}
//	else if (algo_status) {
//		//cout << p0_x << p0_y << p1_x << p1_y;
//		anti_aliasing(p0_x, p0_y, p1_x, p1_y, step);
//	}
//
//	//if (clickstatus == true) {
//	//	glColor3f(0.0f, 1.0f, 1.0f);
//	//	glRectf(drawx, drawy, drawx + step, drawy + step);
//	//	glFlush();
//	//	//cout << "drawx" << drawx << "drawy" << drawy << step / 2.0 << endl;
//	//}
//	glBegin(GL_POINTS);
//	glVertex3f(posX, posY, 0);
//	glEnd();
//
//	glutSwapBuffers();
//
//}
//
//
//void mouseclick(int button, int state, int x, int y) {
//	switch (button)
//	{
//	case GLUT_LEFT_BUTTON:
//		if (state == GLUT_DOWN) {
//			int width = 400, height = 400;
//			double left = -10, right = 10, bottom = -10, top = 10, dnear = -10, dfar = 20;
//
//			posX = ((x - left) / width) * (double)20 - (double)10 - 0.5;
//			posY = -((y - top) / height) * (double)20 + (double)10 - 0.5;
//
//			clickstatus = true;
//
//			float step = 20.0 / grid;
//			int index = -(grid / 2);
//			fillgrid(index, step);
//
//		}
//	default:
//		break;
//	}
//}
//void fillgrid(int index, float step)
//{
//	int x = 0, y = 0;
//	for (float i = -10; i <= 10; i += step) {
//		//判斷點的位置是否在格子裡
//		if ((posX > (i)) && (posX < i + step)) {
//			x = index;
//			p1_x = index;
//			drawx = i;
//		}
//		if ((posY > (i)) && (posY < i + step)) {
//			y = index;
//			p1_y = index;
//			drawy = i;
//		}
//		index++;
//	}
//	cout << "(" << x << "," << y << ")" << endl;
//}
//void drawgrid(float step)
//{
//	for (float i = -10; i <= 10; i += step)
//	{
//		//畫X的線
//		glVertex3f(-10.0f, i, 0.0f);
//		glVertex3f(10.0f, i, 0.0f);
//		//畫Y的線
//		glVertex3f(i, -10.0f, 0.0f);
//		glVertex3f(i, 10.0f, 0.0f);
//	}
//}
//void midpoint(int x0, int y0, int x1, int y1, float step) {
//	if (x1 < x0) {
//		int temp;
//		temp = x0;
//		x0 = x1;
//		x1 = temp;
//		temp = y0;
//		y0 = y1;
//		y1 = temp;
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	float m = 0;
//	if (y1 == y0) {
//		cout << "m=0" << endl;
//
//	}
//	else if (x1 == x0) {
//		cout << "m=無限大" << endl;
//	}
//	else {
//		m = (float)dy / (float)dx;
//		cout << "m=" << m << endl;
//	}
//	if (y1 == y0) {
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (x < x1) {
//			x += 1;
//			setPixel(x, y, step);
//		}
//		while (x > x1) {
//			x -= 1;
//			setPixel(x, y, step);
//		}
//
//	}
//	else if (x1 == x0) {
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (y < y1) {
//			y += 1;
//			setPixel(x, y, step);
//		}
//		while (y > y1) {
//			y -= 1;
//			setPixel(x, y, step);
//		}
//	}
//	else if (m > 0 && m <= 1) {//x方向
//		//cout << "c0" << endl;
//
//		int index;
//		int d = 2 * dy - dx;
//		int delE = 2 * dy;
//		int delNE = 2 * (dy - dx);
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (x < x1) {
//			if (d <= 0) {
//				d += delE;
//				x = x + 1;
//			}
//			else {
//				d += delNE;
//				x = x + 1;
//				y = y + 1;
//			}
//			setPixel(x, y, step);
//		}
//	}
//	else if (m > 1) {//y方向
//		//cout << "c1" << endl;
//		int d = dy - 2 * dx;
//		int delE = -2 * dx;
//		int delNE = 2 * (dy - dx);
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (y < y1) {
//			if (d > 0) {
//				d += delE;
//				y = y + 1;
//			}
//			else {
//				d += delNE;
//				x = x + 1;
//				y = y + 1;
//			}
//			setPixel(x, y, step);
//		}
//	}
//	else if (m < 0 && m >= -1) {//x方向
//		//cout << "c2" << endl;
//
//		int d = 2 * dy + dx;
//		int delE = 2 * dy;
//		int delSE = 2 * (dy + dx);
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (x < x1) {
//			if (d > 0) {
//				d += delE;
//				x = x + 1;
//			}
//			else {
//				d += delSE;
//				x = x + 1;
//				y = y - 1;
//			}
//			setPixel(x, y, step);
//		}
//
//	}
//	else if (m < -1) {//y方向
//		//cout << "c3" << endl;
//
//		int d = dy + 2 * dx;
//		int delE = 2 * dx;
//		int delSE = 2 * (dy + dx);
//		int x = x0, y = y0;
//		setPixel(x, y, step);
//		while (y > y1) {
//			if (d <= 0) {
//				d += delE;
//				y = y - 1;
//			}
//			else {
//				d += delSE;
//				x = x + 1;
//				y = y - 1;
//			}
//			setPixel(x, y, step);
//		}
//	}
//}
//void anti_aliasing(int x0, int y0, int x1, int y1, float step) {
//	if (x1 < x0) {
//		int temp;
//		temp = x0;
//		x0 = x1;
//		x1 = temp;
//		temp = y0;
//		y0 = y1;
//		y1 = temp;
//	}
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	float m = 0;
//	if (y1 == y0) {
//		cout << "m=0" << endl;
//
//	}
//	else if (x1 == x0) {
//		cout << "m=無限大" << endl;
//	}
//	else {
//		m = (float)dy / (float)dx;
//		cout << "m=" << m << endl;
//	}
//	if (y1 == y0) {
//		int x = x0, y = y0;
//		anti_setPixel(x, y, 0, step);
//		while (x < x1) {
//			x += 1;
//			anti_setPixel(x, y, 0, step);
//		}
//		while (x > x1) {
//			x -= 1;
//			anti_setPixel(x, y, 0, step);
//		}
//	}
//	else if (x1 == x0) {
//		int x = x0, y = y0;
//		anti_setPixel(x, y, 0, step);
//		while (y < y1) {
//			y += 1;
//			anti_setPixel(x, y, 0, step);
//		}
//		while (y > y1) {
//			y -= 1;
//			anti_setPixel(x, y, 0, step);
//		}
//	}
//	else if (m > 0 && m <= 1) {//x方向
//		//cout << "c0" << endl;
//		float x = x0, y = y0;
//		for (x = x0; x <= x1; x++) {
//			int yi = floor(y);
//			float f = y - yi;
//			anti_setPixel(x, yi, 1 - f, step);
//			anti_setPixel(x, yi + 1, f, step);
//			y = y + m;
//		}
//
//	}
//	else if (m > 1) {//y方向
//		//cout << "c1" << endl;
//		float x = x0, y = y0;
//		float nm = 1 / m;
//		for (y = y0; y <= y1; y++) {
//			int xi = floor(x);
//			float f = x - xi;
//			anti_setPixel(xi, y, 1 - f, step);
//			anti_setPixel(xi + 1, y, f, step);
//			x = x + nm;
//		}
//	}
//	else if (m < 0 && m >= -1) {//x方向
//		//cout << "c2" << endl;
//		float x = x0, y = y0;
//		for (x = x0; x <= x1; x++) {
//			int yi = floor(y);
//			float f = y - yi;
//			anti_setPixel(x, yi, 1 - f, step);
//			anti_setPixel(x, yi + 1, f, step);
//			y = y + m;
//		}
//	}
//	else if (m < -1) {//y方向
//		float x = x0, y = y0;
//		float nm = 1 / m;
//		for (y = y0; y >= y1; y--) {
//			int xi = floor(x);
//			float f = x - xi;
//			anti_setPixel(xi, y, 1 - f, step);
//			anti_setPixel(xi + 1, y, f, step);
//			x = x - nm;
//		}
//	}
//}
//void setPixel(int x, int y, float step) {
//	glColor3f(0.0f, 1.0f, 1.0f);
//	float point_x = (x * step - step / 2.0);
//	float point_y = (y * step - step / 2.0);
//
//	glRectf(point_x, point_y, point_x + step, point_y + step);
//	glFlush();
//}
//void anti_setPixel(int x, int y, float color, float step) {
//	//cout << x << y << endl;
//	glColor3f(color, color, color);
//
//	float point_x = (x * step - step / 2.0);
//	float point_y = (y * step - step / 2.0);
//
//	glRectf(point_x, point_y, point_x + step, point_y + step);
//	glFlush();
//}
//void demoLine(float step) {
//	glBegin(GL_LINES);
//	float fp0_x = 0.0, fp0_y = 0.0, fp1_x = 0.0, fp1_y = 0.0;
//	if ((p1_x - p0_x) > 0 && (p1_y - p0_y) > 0) {
//		fp0_x = (float)(p0_x)*step - step / 2.0;
//		fp0_y = (float)(p0_y)*step - step / 2.0;
//		fp1_x = (float)(p1_x)*step - step / 2.0 + step;
//		fp1_y = (float)(p1_y)*step - step / 2.0 + step;
//	}
//	else if ((p1_x - p0_x) > 0 && (p1_y - p0_y) < 0) {
//		fp0_x = (float)(p0_x)*step - step / 2.0;
//		fp0_y = (float)(p0_y + 1) * step - step / 2.0;
//		fp1_x = (float)(p1_x)*step - step / 2.0 + step;
//		fp1_y = (float)(p1_y - 1) * step - step / 2.0 + step;
//	}
//	else if ((p1_x - p0_x) < 0 && (p1_y - p0_y) > 0) {
//		fp0_x = (float)(p0_x + 1) * step - step / 2.0;
//		fp0_y = (float)(p0_y)*step - step / 2.0;
//		fp1_x = (float)(p1_x - 1) * step - step / 2.0 + step;
//		fp1_y = (float)(p1_y)*step - step / 2.0 + step;
//	}
//	else if ((p1_x - p0_x) < 0 && (p1_y - p0_y) < 0) {
//		fp0_x = (float)(p0_x + 1) * step - step / 2.0;
//		fp0_y = (float)(p0_y + 1) * step - step / 2.0;
//		fp1_x = (float)(p1_x - 1) * step - step / 2.0 + step;
//		fp1_y = (float)(p1_y - 1) * step - step / 2.0 + step;
//	}
//	glVertex3f(fp0_x, fp0_y, 0);
//	glVertex3f(fp1_x, fp1_y, 0);
//	glEnd();
//}
