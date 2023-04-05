#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <.\GL\freeglut.h>
using namespace std;
#define PI 3.1415926
class vertex_attr {
public:
    GLfloat x, y, z;
    vertex_attr(GLfloat _x, GLfloat _y, GLfloat _z);
    vertex_attr();
};
class obj_parser {
public:
    vector<vertex_attr> vertex;
    vector<vertex_attr> vertex_origin;
    vector<GLint> surface;
    vector<vertex_attr> bonding_point_origin;
    vector<vertex_attr> bonding_point;
    obj_parser(string filePath);
    GLfloat width, height, depth;
private:
    string input = "";
    string::size_type pos = 0;
    GLfloat mid_x=0, mid_y=0, mid_z=0;
    GLfloat scale_factor=1.0f;
    void load();
    void nextfloat();
    void nextindex();
    void skipWhiteSpace();
    void findboudingbox();
    void scale();
    void translate();
};