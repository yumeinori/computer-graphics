#include <stdio.h> 
#include <stdlib.h>
#include <.\GL\freeglut.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <time.h>
class vertex {
public:
	float x, y, r, g, b;
	vertex(float _x, float _y, float _r, float _g, float _b) :x(_x), y(_y), r(_r), g(_g), b(_b) {}
	vertex() {
		x = 0;
		y = 0;
		r = 0;
		g = 0;
		b = 0;
	}
};
using namespace std;
void myKeyboard(unsigned char key, int x, int y);
void ChangeSize(int, int);
void RenderScene(void);
void mouseclick(int, int, int, int);
void SetupRC();
void buildPopupMenu();
void drawgrid(float);
void fillgrid(int, float);
void midpoint(vertex v0, vertex v1);
void setPixel(int x, int y);
void crow(vector<vertex> vtList);
void scanY(vector<vertex> v, int i);
void scanX(vertex* l, vertex* r, int y);
void differenceY(vertex* v1, vertex* v2, vertex* e, vertex* de, int y);
void differenceX(vertex* v1, vertex* v2, vertex* e, vertex* de, int x);
void difference(vertex* v1, vertex* v2, vertex* e, vertex* de, float d, float f);
void increment(vertex* v, vertex* dv);
bool test(int ly, int rn, int ln, int i, vector<vertex> v, int y);
bool testnode();
//滑鼠位置
float posX = 0, posY = 0, posZ = 0;
//點擊狀態
bool clickstatus = false;
//現在使用的演算法
bool algo_status = false;
float drawx = 0, drawy = 0;
#define PI 3.14159
//格子數
float grid = 11.0;
//每格的長度
float step = 0;
//頂點
vector<vertex> vList;
//是否要顯示
bool show = false;

//輸入的兩點
int p0_x = 0, p0_y = 0, p1_x = -5, p1_y = -2;
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(600, 80);
	glutCreateWindow("Moving Cube");

	SetupRC();
	glutKeyboardFunc(myKeyboard);
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
		grid = 11.0;
		glutPostRedisplay();
		break;
	case 1:
		clickstatus = false;
		grid = 15.0;
		glutPostRedisplay();
		break;
	case 2:
		clickstatus = false;
		grid = 21.0;
		glutPostRedisplay();
		break;
	case 3:
		algo_status = false;
		glutPostRedisplay();
		break;
	case 4:
		algo_status = true;
		glutPostRedisplay();
		break;
	case 5:
		glColor3f(0.0f, 0.0f, 0.0f);
		algo_status = false;
		if (vList.size()) {
			for (int i = 0; i < vList.size() - 1; i++) {
				midpoint(vList[i], vList[i + 1]);
			}
			vList.clear();
			show = false;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}
void myKeyboard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 's':
		show = true;
		break;
		glutPostRedisplay();
	}
}
void buildPopupMenu()
{
	int menu_id;
	menu_id = glutCreateMenu(menuSelect);
	glutAddMenuEntry("11", 0);
	glutAddMenuEntry("15", 1);
	glutAddMenuEntry("21", 2);
	glutAddMenuEntry("Rasterization lines", 3);
	glutAddMenuEntry("Rasterization polygon", 4);
	glutAddMenuEntry("Reset", 5);
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
	step = 20.0 / grid;
	drawgrid(step);

	glEnd();
	//畫演示線
	////////////////////////////////////////
	//畫線演算法
	if (!algo_status) {
		if (vList.size() == 1) {
			glColor3f(vList[0].r, vList[0].g, vList[0].b);
			setPixel(vList[0].x, vList[0].y);
		}
		else if (vList.size() > 1) {
			for (int i = 0; i < vList.size(); i++) {
				if (i < vList.size() - 1) {
					glColor3f(vList[i].r, vList[i].g, vList[i].b);
					midpoint(vList[i], vList[i + 1]);
				}
			}
		}
	}
	else if (algo_status) {
		if (vList.size() >= 3) {
			crow(vList);
		}
		else {
			for (int i = 0; i < vList.size(); i++)
			{
				glColor3f(vList[i].r, vList[i].g, vList[i].b);
				setPixel(vList[i].x, vList[i].y);
			}
		}
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

			float step = 20.0 / grid;
			int index = -(grid / 2);
			fillgrid(index, step);

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
			p1_x = index;
			drawx = i;
		}
		if ((posY > (i)) && (posY < i + step)) {
			y = index;
			p1_y = index;
			drawy = i;
		}
		index++;
	}
	srand(time(NULL));
	float min = 0;
	float max = 1.0;
	float r = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	float g = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	float b = (max - min) * rand() / (RAND_MAX + 1.0) + min;
	vertex v((float)x, (float)y, r, g, b);
	vList.push_back(v);
	cout << "(" << x << "," << y << ")" << endl;
	cout << "r:" << r << "g:" << g << "b:" << b << endl;
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
void midpoint(vertex v0, vertex v1) {
	int x0 = (int)v0.x;
	int x1 = (int)v1.x;
	int y0 = (int)v0.y;
	int y1 = (int)v1.y;
	float r0 = v0.r;
	float r1 = v1.r;
	float g0 = v0.g;
	float g1 = v1.g;
	float b0 = v0.b;
	float b1 = v1.b;

	if (x1 < x0) {
		int temp;
		temp = x0;
		x0 = x1;
		x1 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;

		float colortemp;
		colortemp = r0;
		r0 = r1;
		r1 = colortemp;

		colortemp = g0;
		g0 = g1;
		g1 = colortemp;

		colortemp = b0;
		b0 = b1;
		b1 = colortemp;
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	float m = 0;
	if (y1 == y0) {
		//cout << "m=0" << endl;

	}
	else if (x1 == x0) {
		//cout << "m=無限大" << endl;
	}
	else {
		m = (float)dy / (float)dx;
		//cout << "m=" << m << endl;
	}
	if (y1 == y0) {
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);

		setPixel(x, y);
		int color_x = x0;
		while (x < x1) {
			x += 1;
			float r = r0 + dr * (x - color_x);
			float g = g0 + dg * (x - color_x);
			float b = b0 + db * (x - color_x);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
		while (x > x1) {
			x -= 1;
			float r = r0 + dr * (color_x - x);
			float g = g0 + dg * (color_x - x);
			float b = b0 + db * (color_x - x);
			glColor3f(r, g, b);
			setPixel(x, y);
		}

	}
	else if (x1 == x0) {
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);

		setPixel(x, y);
		int color_y = y0;
		while (y < y1) {
			y += 1;
			float r = r0 + dr * (y - color_y);
			float g = g0 + dg * (y - color_y);
			float b = b0 + db * (y - color_y);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
		while (y > y1) {
			y -= 1;
			float r = r0 + dr * (y - color_y);
			float g = g0 + dg * (y - color_y);
			float b = b0 + db * (y - color_y);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
	}
	else if (m > 0 && m <= 1) {//x方向
		//cout << "c0" << endl;

		int index;
		int d = 2 * dy - dx;
		int delE = 2 * dy;
		int delNE = 2 * (dy - dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);

		setPixel(x, y);
		int color_x = x0;
		while (x < x1) {
			if (d <= 0) {
				d += delE;
				x = x + 1;
			}
			else {
				d += delNE;
				x = x + 1;
				y = y + 1;
			}

			float r = r0 + dr * (x - color_x);
			float g = g0 + dg * (x - color_x);
			float b = b0 + db * (x - color_x);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
	}
	else if (m > 1) {//y方向
		//cout << "c1" << endl;
		int d = dy - 2 * dx;
		int delE = -2 * dx;
		int delNE = 2 * (dy - dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);
		setPixel(x, y);
		int color_y = y0;

		while (y < y1) {
			if (d > 0) {
				d += delE;
				y = y + 1;
			}
			else {
				d += delNE;
				x = x + 1;
				y = y + 1;
			}


			float r = r0 + dr * (y - color_y);
			float g = g0 + dg * (y - color_y);
			float b = b0 + db * (y - color_y);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
	}
	else if (m < 0 && m >= -1) {//x方向
		//cout << "c2" << endl;

		int d = 2 * dy + dx;
		int delE = 2 * dy;
		int delSE = 2 * (dy + dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (x0 - x1);
		float dg = (g0 - g1) / (x0 - x1);
		float db = (b0 - b1) / (x0 - x1);
		setPixel(x, y);
		int color_x = x0;

		while (x < x1) {
			if (d > 0) {
				d += delE;
				x = x + 1;
			}
			else {
				d += delSE;
				x = x + 1;
				y = y - 1;
			}
			float r = r0 + dr * (x - color_x);
			float g = g0 + dg * (x - color_x);
			float b = b0 + db * (x - color_x);
			glColor3f(r, g, b);
			setPixel(x, y);
		}

	}
	else if (m < -1) {//y方向
		//cout << "c3" << endl;

		int d = dy + 2 * dx;
		int delE = 2 * dx;
		int delSE = 2 * (dy + dx);
		int x = x0, y = y0;
		glColor3f(r0, g0, b0);
		float dr = (r0 - r1) / (y0 - y1);
		float dg = (g0 - g1) / (y0 - y1);
		float db = (b0 - b1) / (y0 - y1);
		setPixel(x, y);
		int color_y = y0;

		while (y > y1) {
			if (d <= 0) {
				d += delE;
				y = y - 1;
			}
			else {
				d += delSE;
				x = x + 1;
				y = y - 1;
			}
			float r = r0 + dr * (y - color_y);
			float g = g0 + dg * (y - color_y);
			float b = b0 + db * (y - color_y);
			glColor3f(r, g, b);
			setPixel(x, y);
		}
	}
}
void setPixel(int x, int y) {
	float point_x = (x * step - step / 2.0);
	float point_y = (y * step - step / 2.0);
	glRectf(point_x, point_y, point_x + step, point_y + step);
	glFlush();
}
void crow(vector<vertex> vtList) {
	int iMin = 0;
	for (int i = 1; i < vtList.size(); i++) {
		if (vtList[i].y < vtList[0].y) {
			iMin = i;
		}
		else if (vtList[i].y < vtList[iMin].y) {
			iMin = i;
		}
	}
	//cout << "最下面" << vtList[iMin].y << endl;
	scanY(vtList, iMin);
}
void scanY(vector<vertex> v, int i) {
	int li, ri;		//左右點的index
	int ly, ry;		//左右點的y值
	vertex l, dl;	//現在左邊的點跟delta
	vertex r, dr;	//現在右邊的點跟delta
	int rem;		//剩餘的頂點數
	int y;			//現在的掃描線位置

	int n = v.size();

	li = ri = i;
	ly = ry = y = (int)ceil(v[i].y);

	for (rem = n; rem > 0;) {
		//找左邊的點
		while (ly <= y && rem > 0)
		{
			rem--;
			i = li - 1;
			if (i < 0) {
				i = n - 1;
			}
			ly = (int)ceil(v[i].y);
			//cout << "找左邊" << rem << ":::" << "y:" << y << "ly:" << ly << "選到的頂點" << v[i].x << v[i].y << endl;
			if (ly > y) {
				differenceY(&v[li], &v[i], &l, &dl, y);
			}
			//凹面
			if (n > 4) {
				int ln, rn;
				if (i == (n - 1)) {
					rn = 0;
					ln = i - 1;
				}
				else if (i == 0) {
					rn = i + 1;
					ln = n - 1;
				}
				else {
					rn = i + 1;
					ln = i - 1;
				}
				if (ly < y && (v[rn].x > v[ln].x) && (v[rn].y > v[i].y) && (v[ln].y > v[i].y)) {//在現在的點右邊
					//cout << "找左邊" << endl;

					vector<vertex> outList;
					vertex temp;
					float m, r, g, b, temp_x, d, dify;
					if (y > v[ln].y) {
						m = (v[ln].x - v[i].x) / (v[ln].y - v[i].y);
						d = v[i].y - v[ln].y;
						dify = y - v[i].y;
						if (v[i].x == v[ln].x) {
							temp.x = v[i].x;
						}
						else {
							temp.x = v[rn].x;
						}
						temp.y = y;
						temp.r = v[i].r + (v[i].r - v[ln].r) / d * dify;
						temp.g = v[i].g + (v[i].g - v[ln].g) / d * dify;
						temp.b = v[i].b + (v[i].b - v[ln].b) / d * dify;
						//放入右邊的點補償
						vertex temp2;
						temp2.x = v[rn].x;
						temp2.y = y;
						temp2.r = v[rn].r;
						temp2.g = v[rn].g;
						temp2.b = v[rn].b;
						outList.push_back(v[rn]);
						outList.push_back(v[i]);
						outList.push_back(temp);
						outList.push_back(temp2);
						crow(outList);
						outList.clear();
					}
					m = (v[ln].x - v[i].x) / (v[ln].y - v[i].y);
					d = v[i].y - v[ln].y;
					dify = v[rn].y - v[i].y;
					if (v[i].x == v[ln].x) {
						temp.x = v[i].x;
					}
					else {
						temp.x = m * v[rn].y + v[ln].x;
					}
					temp.y = v[rn].y;
					temp.r = v[i].r + (v[i].r - v[ln].r) / d * dify;
					temp.g = v[i].g + (v[i].g - v[ln].g) / d * dify;
					temp.b = v[i].b + (v[i].b - v[ln].b) / d * dify;
					outList.push_back(v[rn]);
					outList.push_back(v[i]);
					outList.push_back(temp);


					/*cout << "node:" << "第一點:" << v[rn].x << "===" << v[rn].y << endl;
					cout << "r:" << v[rn].r << "g:" << v[rn].g << "b:" << v[rn].b << endl;
					cout << "node:" << "第二點:" << v[i].x << "===" << v[i].y << endl;
					cout << "r:" << v[i].r << "g:" << v[i].g << "b:" << v[i].b << endl;
					cout << "node:" << "第三點:" << temp.x << "===" << temp.y << endl;
					cout << "temp:" << "r:" << temp.r << "g:" << temp.g << "b:" << temp.b << endl;
					cout << "temp:" << "d:" << d << "dify:" << dify << endl;*/
					crow(outList);
					outList.clear();
				}

			}
			li = i;//左邊點的index 
			//cout << "找左邊" << l.x << r.x << endl;
		}
		//找右邊的點
		while (ry <= y && rem > 0) {
			rem--;
			i = ri + 1;
			if (i >= n) {
				i = 0;
			}
			ry = (int)ceil(v[i].y);
			//cout << "找右邊" << rem << ":::" << "y:" << y << "ry:" << ly << "選到的頂點" << v[i].x << v[i].y << endl;
			if (ry > y) {
				differenceY(&v[ri], &v[i], &r, &dr, y);
			}
			//凹面
			if (n > 4) {
				int ln, rn;
				if (i == (n - 1)) {
					rn = 0;
					ln = i - 1;
				}
				else if (i == 0) {
					rn = i + 1;
					ln = n - 1;
				}
				else {
					rn = i + 1;
					ln = i - 1;
				}
				if (ry < y && (v[rn].x > v[ln].x) && (v[rn].y > v[i].y) && (v[ln].y > v[i].y)) {
					//cout << "找右邊" << endl;
					vector<vertex> outList;
					vertex temp;
					float m, r, g, b, temp_x, d, dify;
					//判斷右邊的點是不是低於掃描線y
					if (y > v[rn].y) {
						/*
						---------------------------scan_y

						x<-------rn

						x<-----i
						*/
						///
						//cout << "低於掃描" << endl;
						m = (v[rn].x - v[i].x) / (v[rn].y - v[i].y);
						d = v[i].y - v[rn].y;
						dify = y - v[i].y;
						if (v[i].x == v[rn].x) {
							temp.x = v[i].x;
						}
						else {
							temp.x = v[rn].x;
						}
						temp.y = y;
						temp.r = v[i].r + (v[i].r - v[rn].r) / d * dify;
						temp.g = v[i].g + (v[i].g - v[rn].g) / d * dify;
						temp.b = v[i].b + (v[i].b - v[rn].b) / d * dify;
						//放入左邊的點補償
						vertex temp2;
						temp2.x = v[ln].x;
						temp2.y = y;
						temp2.r = v[ln].r;
						temp2.g = v[ln].g;
						temp2.b = v[ln].b;
						outList.push_back(v[ln]);
						outList.push_back(v[i]);
						outList.push_back(temp);
						outList.push_back(temp2);
						crow(outList);
						outList.clear();
					}

					m = (v[rn].x - v[i].x) / (v[rn].y - v[i].y);
					d = v[i].y - v[rn].y;
					dify = v[ln].y - v[i].y;
					if (v[i].x == v[rn].x) {
						temp.x = v[i].x;
					}
					else {
						temp.x = m * v[ln].y + v[rn].x;
					}
					temp.y = v[ln].y;
					temp.r = v[i].r + (v[i].r - v[rn].r) / d * dify;
					temp.g = v[i].g + (v[i].g - v[rn].g) / d * dify;
					temp.b = v[i].b + (v[i].b - v[rn].b) / d * dify;
					outList.push_back(v[ln]);
					outList.push_back(v[i]);
					outList.push_back(temp);


					/*cout << "2:" << "r:" << v[rn].r << "g:" << v[rn].g << "b:" << v[rn].b << endl;
					cout << "1:" << "r:" << v[i].r << "g:" << v[i].g << "b:" << v[i].b << endl;
					cout << "temp:" << "r:" << temp.r << "g:" << temp.g << "b:" << temp.b << endl;
					cout << "temp:" << "d:" << d << "dify:" << dify << endl;*/

					/*	cout << "node:" << "第一點:" << v[ln].x << "===" << v[ln].y << endl;
						cout << "node:" << "第二點:" << v[i].x << "===" << v[i].y << endl;
						cout << "node:" << "第三點:" << temp.x << "===" << temp.y << endl;*/

					crow(outList);
					outList.clear();
				}
			}
			ri = i;
			//cout << "找右邊" << l.x << r.x << endl;
		}
		//填色
		float color_y = (float)y;
		/*cout << "v1_x:" << v[0].x << "v1_y:" << v[0].y  << "v1_r:" << v[0].r << "v1_g:" << v[0].g << "v1_b:" << v[0].b << endl;
		cout << "v2_x:" << v[1].x << "v2_y:" << v[1].y << "v2_r:" << v[1].r << "v2_g:" << v[1].g << "v2_b:" << v[1].b << endl;
		cout << "v3_x:" << v[2].x << "v3_y:" << v[2].y << "v3_r:" << v[2].r << "v3_g:" << v[2].g << "v3_b:" << v[2].b << endl;
		cout << "dr" << dl.r << endl;
		cout << l.r << endl;
		cout << r.r << endl;*/

		for (; y <= ly && y <= ry; y++) {
			//cout << "填色" << l.x << r.x << endl;
			/*cout << "填色y" << y <<endl;
			cout << "填色" << endl;*/
			/*cout << "l.r" << l.r << " " << dl.r << " " << y << " " << color_y  << endl;
			cout << "r.r" << r.r << " " << dr.r << " " << y << " " << color_y << endl;*/

			scanX(&l, &r, y);
			increment(&l, &dl);
			increment(&r, &dr);
		}
	}
}
void scanX(vertex* l, vertex* r, int y) {

	int x, lx, rx;
	vertex s, ds;

	lx = (int)ceil(l->x);
	rx = (int)ceil(r->x);
	if (lx <= rx) {
		differenceX(l, r, &s, &ds, lx);
		for (x = lx; x <= rx; x++) {
			/*cout << "scanXs:" << s.r << " " << s.g << " " << s.b << endl;
			cout << "scanXds:" << ds.r << " " << ds.g << " " << ds.b << endl;*/
			/*cout << "2_l.r" << l->r << endl;
			cout << "2_l.g" << l->g << endl;
			cout << "2_l.b" << l->b << endl;*/

			//cout << "ds.r" << ds.r << endl;

			if (x == lx) {
				glColor3f(l->r, l->g, l->b);
				setPixel(x, y);
				/*cout << "node:" << "點:" << x << "===" << y << endl;
				cout << "r:" << l->r << "g:" << l->g << "b:" << l->b << endl;*/
			}
			else {
				increment(&s, &ds);
				setPixel(x, y);
				/*cout << "node:" << "點:" << x << "===" << y << endl;
				cout << "r:" << s.r << "g:" << s.g << "b:" << s.b << endl;*/
			}

		}
	}
	else {
		differenceX(r, l, &s, &ds, rx);
		for (x = rx; x <= lx; x++) {
			//cout << "2_r.r" << r->r << endl;
			if (x == rx) {
				glColor3f(r->r, r->g, r->b);
				setPixel(x, y);
				/*cout << "node:" << "點:" << x << "===" << y << endl;
				cout << "r:" << r->r << "g:" << r->g << "b:" << r->b << endl;*/
			}
			else {
				increment(&s, &ds);
				setPixel(x, y);
				/*cout << "node:" << "點:" << x << "===" << y << endl;
				cout << "r:" << s.r << "g:" << s.g << "b:" << s.b << endl;*/
			}

		}
	}

}
void differenceY(vertex* v1, vertex* v2, vertex* e, vertex* de, int y)
{
	//上面的點-底下的點

	float f = y - v1->y;
	float d = v2->y - v1->y;
	//cout << "differenceY" << ":::" << "v2->y" << v2->y << "   v1->y" << v1->y << endl;
	//cout << "differenceY" << ":::" << "y" << y << "v1y" << v1->y << "f:" << f << "d:" << d << endl;
	difference(v1, v2, e, de, d, f);
}
void differenceX(vertex* v1, vertex* v2, vertex* e, vertex* de, int x)
{
	//右邊的點-左邊的點

	float f = x - v1->x;
	float d = v2->x - v1->x;
	//cout << "differenceX" << ":::" << "f:" << f << "d:" << d << endl;
	difference(v1, v2, e, de, d, f);
}
void difference(vertex* v1, vertex* v2, vertex* e, vertex* de, float d, float f)
{
	de->x = (v2->x - v1->x) / d;//每+一次y要多多少x
	e->x = v1->x + f * (de->x);//補償x
	//cout << "difference" << ":::" << "de:" << de->x << "e:" << e->x << endl;
	de->r = (v1->r - v2->r) / (-d);
	e->r = (v1->r) + f * (de->r);
	de->g = (v1->g - v2->g) / (-d);
	e->g = v1->g + f * (de->g);
	de->b = (v1->b - v2->b) / (-d);
	e->b = v1->b + f * (de->b);
	/*cout << "v1_r" << ":" <<  v1->r << endl;
	cout << "v2_r" << ":" <<  v2->r << endl;
	cout << "difference_r" << ":::" << "de:" << de->r << " e:" << e->r << endl << endl;
	cout << "v1_g" << ":" << v1->g << endl;
	cout << "v2_g" << ":" << v2->g << endl ;
	cout << "difference_g" << ":::" << "de:" << de->g << " e:" << e->g << endl << endl;
	cout << "v1_b" << ":" << v1->b  << endl;
	cout << "v2_b" << ":" << v2->b  << endl ;
	cout << "difference_b" << ":::" << "de:" << de->b << " e:" << e->b << endl << endl;*/
}
void increment(vertex* v, vertex* dv) {
	v->x += dv->x;
	v->r += dv->r;
	v->g += dv->g;
	v->b += dv->b;
	//cout << "incrementr" << v->r<<endl;
	//cout << "incrementg" << v->g<< endl;
	//cout << "incrementb" << v->b << endl;

	glColor3f(v->r, v->g, v->b);
}
bool test(int ly, int rn, int ln, int i, vector<vertex> v, int y) {
	bool temp = false;
	cout << "rnnfk:" << rn << endl;
	vertex rnn = v[rn];
	cout << "lnnfk:" << ln << endl;
	vertex lnn = v[ln];
	cout << "iii" << i << endl;
	vertex iii = v[i];

	return true;
}
bool testnode() {
	vertex v(-7.0, -8.0, 0.484039, 0.877716, 0.0606384);
	v.x = -8;
	v.y = -8;
	v.r = 0.762573;
	v.g = 0.680573;
	v.b = 0.81427;
	vList.push_back(v);

	v.x = -4;
	v.y = 2;
	v.r = 0.762665;
	v.g = 0.00860596;
	v.b = 0.359436;
	vList.push_back(v);

	v.x = 0;
	v.y = -5;
	v.r = 0.762665;
	v.g = 0.00860596;
	v.b = 0.359436;
	vList.push_back(v);

	v.x = 2;
	v.y = -1;
	v.r = 0.762665;
	v.g = 0.00860596;
	v.b = 0.359436;
	vList.push_back(v);

	v.x = 6;
	v.y = -7;
	v.r = 0.76297;
	v.g = 0.992645;
	v.b = 0.994965;
	vList.push_back(v);

	v.x = 8;
	v.y = -3;
	v.r = 0.763062;
	v.g = 0.320648;
	v.b = 0.540131;
	vList.push_back(v);

	v.x = 8;
	v.y = 8;
	v.r = 0.763184;
	v.g = 0.648682;
	v.b = 0.0852966;
	vList.push_back(v);

	v.x = -9;
	v.y = 7;
	v.r = 0.763275;
	v.g = 0.976685;
	v.b = 0.630463;
	vList.push_back(v);
	return true;
}


