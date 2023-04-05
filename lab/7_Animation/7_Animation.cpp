// SphereWorld.cpp
// OpenGL SuperBible
// Demonstrates an immersive 3D environment using actors
// and a camera. This version adds lights and material properties
// and shadows.
// Program by Richard S. Wright Jr.
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <.\GL\freeglut.h>
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;
class GLFrame 
{
protected:
    GLfloat vOrigin[3];	// Where am I?
    GLfloat vForward[3];	// Where am I going?
    GLfloat vUp[3];
    GLfloat angle;
public:

    GLFrame(void) {
        // At origin
        vOrigin[0] = 0.0f; vOrigin[1] = 0.0f; vOrigin[2] = 0.0f;

        // Up is up (+Y)
        vUp[0] = 0.0f; vUp[1] = 1.0f; vUp[2] = 0.0f;

        // Forward is -Z (default OpenGL)
        vForward[0] = 0.0f; vForward[1] = 0.0f; vForward[2] = -1.0f;
        
    }
    void SetOrigin(float x, float y, float z) {
        vOrigin[0] = x; vOrigin[1] = y; vOrigin[2] = z;
    }
    void MoveForward(float fDelta)
    {
		// Move along direction of front direction
		vOrigin[0] = vOrigin[0] + vForward[0] * fDelta;
		//vOrigin[1] += vForward[1] * fDelta;
		vOrigin[2] = vOrigin[2] + vForward[2] * fDelta;
        glLoadIdentity();
        gluLookAt(vOrigin[0], vOrigin[1], vOrigin[2], vOrigin[0]+ vForward[0], vOrigin[1]+ vForward[1], vOrigin[2]+ vForward[2], 0.0f, 1.0f, 0.0f);
    }
    void RotateLocalY(float fDelta)
    {
        angle += fDelta*10;
        vForward[0] = sin(angle*3.1415926/180);
        vForward[2] = -cos(angle * 3.1415926 / 180);
        glLoadIdentity();
        gluLookAt(vOrigin[0], vOrigin[1], vOrigin[2], vOrigin[0] + vForward[0], vOrigin[1] + vForward[1], vOrigin[2] + vForward[2], 0.0f, 1.0f, 0.0f);
    }
};

#define NUM_SPHERES      30
GLFrame    spheres[NUM_SPHERES];
GLFrame    frameCamera;
int timer = 0;
// Light and material Data
GLfloat fLightPos[4]   = { -100.0f, 100.0f, 50.0f, 1.0f };  // Point source
GLfloat fNoLight[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat fLowLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat fBrightLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat mShadowMatrix[]= { 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

#define GROUND_TEXTURE  0
#define TORUS_TEXTURE   1
#define SPHERE_TEXTURE  2
#define NUM_TEXTURES    3
GLuint  textureObjects[NUM_TEXTURES];

#define idle  0
#define anim_leg   1
#define anim_arms  2
#define anim_torso  3

int anim_flag = idle;
const char *szTextureFiles[] = {"D:/program/cg-hw/hw10/lab10/grass.jpg", "D:/program/cg-hw/hw10/lab10/wood.jpg", "D:/program/cg-hw/hw10/lab10/orb.jpg"};

void m3dFindNormal(GLfloat result[9], const GLfloat point1[3], const GLfloat point2[3], const GLfloat point3[3]);
void m3dGetPlaneEquation(GLfloat planeEq[4], const GLfloat p1[3], const GLfloat p2[3], const GLfloat p3[3]);
void m3dMakePlanarShadowMatrix(GLfloat proj[16], const GLfloat planeEq[4], const GLfloat vLightPos[3]);
void m3dCrossProduct(GLfloat result[9], const GLfloat v1[3], const GLfloat v2[3]);
void m3dNormalizeVector(GLfloat result[9]);
void m3dCrossProduct4(GLfloat result[4], const GLfloat v1[3], const GLfloat v2[3]);
void m3dNormalizeVector4(GLfloat result[4]);
void drawTorus(double r, double c, int rSeg, int cSeg, GLuint texture);
void myKeyboard(unsigned char, int, int);
void TimerFunction(int value);
void drawRightleg(float);
void drawLeftleg(float);
void drawRighthand(float);
void drawLefthand(float);
//////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering
// context. 
void menuSelect(int option)
{
    switch (option)
    {
    case 0:
        anim_flag = idle;        
        glutPostRedisplay();
        break;
    case 1:
        anim_flag = anim_arms;
        glutPostRedisplay();
        break;
    case 2:
        anim_flag = anim_torso;
        glutPostRedisplay();
        break;
    case 3:
        anim_flag = anim_leg;
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
    glutAddMenuEntry("idle", 0);
    glutAddMenuEntry("arms", 1);
    glutAddMenuEntry("torso", 2);
    glutAddMenuEntry("legs", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void SetupRC()
    {
    GLfloat vPoints[3][3] = {{ 0.0f, -0.4f, 0.0f },
                             { 10.0f, -0.4f, 0.0f },
                             { 5.0f, -0.4f, -5.0f }};
    int iSphere;
    int i;
    
    // Grayish background
    glClearColor(fLowLight[0], fLowLight[1], fLowLight[2], fLowLight[3]);
   
    // Clear stencil buffer with zero, increment by one whenever anybody
    // draws into it. When stencil function is enabled, only write where
    // stencil value is zero. This prevents the transparent shadow from drawing
    // over itself
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
    glClearStencil(0);
    glStencilFunc(GL_EQUAL, 0x0, 0x01);
    
    // Cull backs of polygons
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_MULTISAMPLE_ARB);
    
    // Setup light parameters
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, fNoLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fLowLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fBrightLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, fBrightLight);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
        
    // Calculate shadow matrix
    GLfloat pPlane[4];
    m3dGetPlaneEquation(pPlane, vPoints[0], vPoints[1], vPoints[2]);
    m3dMakePlanarShadowMatrix(mShadowMatrix, pPlane, fLightPos);
    
    // Mostly use material tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
    glMateriali(GL_FRONT, GL_SHININESS, 128);
  
    
    // Randomly place the sphere inhabitants
    for(iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
        {
        // Pick a random location between -20 and 20 at .1 increments
        spheres[iSphere].SetOrigin(((float)((rand() % 400) - 200) * 0.1f), 0.0, (float)((rand() % 400) - 200) * 0.1f);
        }
      
    // Set up texture maps
    glEnable(GL_TEXTURE_2D);
    glGenTextures(NUM_TEXTURES, textureObjects);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    
    for(i = 0; i < NUM_TEXTURES; i++)
        {

        Mat image = imread(szTextureFiles[i]);
        cout << "no image" <<szTextureFiles[i]<< endl;
        if (image.empty()) {
            cout << "no image" << endl;
        }
        glBindTexture(GL_TEXTURE_2D, textureObjects[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        #define GL_CLAMP_TO_EDGE 0x812F
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D,
            0,
            GL_RGB,
            image.cols,
            image.rows,
            0,
            GL_BGR_EXT,
            GL_UNSIGNED_BYTE,
            image.ptr()
        );
        }

    }

////////////////////////////////////////////////////////////////////////
// Do shutdown for the rendering context
void ShutdownRC(void)
    {
    // Delete the textures
    glDeleteTextures(NUM_TEXTURES, textureObjects);
    }


///////////////////////////////////////////////////////////
// Draw the ground as a series of triangle strips
void DrawGround(void)
    {
    GLfloat fExtent = 20.0f;
    GLfloat fStep = 1.0f;
    GLfloat y = -0.4f;
    GLfloat iStrip, iRun;
    GLfloat s = 0.0f;
    GLfloat t = 0.0f;
    GLfloat texStep = 1.0f / (fExtent * .075f);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureObjects[GROUND_TEXTURE]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    glBegin(GL_TRIANGLE_STRIP);
    for(iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
        {
        t = 0.0f;
        glBegin(GL_TRIANGLE_STRIP);

            for(iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
                {
                glTexCoord2f(s, t);
                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
                glVertex3f(iStrip, y, iRun);
                
                glTexCoord2f(s + texStep, t);
                glNormal3f(0.0f, 1.0f, 0.0f);   // All Point up
                glVertex3f(iStrip + fStep, y, iRun);
                
                t += texStep;
                }
        glEnd();
        s += texStep;
        }
    }
void DrawRobot(GLint nShadow) {
    static GLfloat leg = 0.0f;
    static GLint leg_flag = 0;

    if (nShadow == 0)
    {
        if (leg == 45) 
        {
            leg_flag = 1;
        }
        else if (leg == -45)
        {
            leg_flag = 0;
        }
        if (leg_flag==0)
        {
            leg += 0.5f;
        }
        else if(leg_flag == 1)
        {
            leg -= 0.5f;
        }
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
        //body
        
        glTranslatef(0.0f, 0.0f, -1.25f);
        if (anim_flag == anim_torso) {
            glRotatef(-leg * 2.0f, 0.0f, 1.0f, 0.0f);
        }
        glutSolidCube(0.2f);
        glTranslatef(0.0f, 0.1f, 0.0f);
        glutSolidCube(0.2f);
        drawRightleg(leg);
        drawLeftleg(-leg);
        drawRighthand(-leg);
        drawLefthand(leg);
        //head
        glTranslatef(0.0f, 0.12f, 0.0f);
        glutSolidCube(0.1f);

        
    glPopMatrix();
}
///////////////////////////////////////////////////////////////////////
// Draw random inhabitants and the rotating torus/sphere duo
void DrawInhabitants(GLint nShadow)
    {
    static GLfloat yRot = 0.0f;         // Rotation angle for animation
    GLint i;

    if(nShadow == 0)
        {
        yRot += 0.5f;
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }
    else
        glColor4f(0.00f, 0.00f, 0.00f, .6f);  // Shadow color
  
        
    // Draw the randomly located spheres
    glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
    for(i = 0; i < NUM_SPHERES; i++)
        {
        glPushMatrix();
        //spheres[i].ApplyActorTransform();
        GLUquadric *quad= gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
        gluQuadricTexture(quad, GL_TRUE);//瞶ㄧ计
        gluSphere(quad, 0.3f, 21, 11);
        //glutSolidSphere(0.3f, 21, 11);
        glPopMatrix();
        }

    glPushMatrix();
        glTranslatef(0.0f, 0.1f, -2.5f);
    
        glPushMatrix();
            glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
            glTranslatef(1.0f, 0.0f, 0.0f);
            GLUquadric* quad = gluNewQuadric();
            glEnable(GL_TEXTURE_2D);
            gluQuadricTexture(quad, GL_TRUE);//瞶ㄧ计
            gluSphere(quad, 0.1f, 21, 11);
        glPopMatrix();
    
        if(nShadow == 0)
            {
            // Torus alone will be specular
            glMaterialfv(GL_FRONT, GL_SPECULAR, fBrightLight);
            }
        
        glRotatef(yRot, 0.0f, 1.0f, 0.0f);
        //glBindTexture(GL_TEXTURE_2D, textureObjects[TORUS_TEXTURE]);
        //glutWireTorus(0.35, 0.15, 61, 37);
        drawTorus(0.05, 0.13,61, 37,textureObjects[TORUS_TEXTURE]);
        glMaterialfv(GL_FRONT, GL_SPECULAR, fNoLight);
    glPopMatrix();
    }

        
// Called to draw scene
void RenderScene(void)
    {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
    glPushMatrix();
        
        // Position light before any other transformations
        glLightfv(GL_LIGHT0, GL_POSITION, fLightPos);
        
        // Draw the ground
        glColor3f(1.0f, 1.0f, 1.0f);
        DrawGround();
        
        // Draw shadows first
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_STENCIL_TEST);
        glPushMatrix();
            glMultMatrixf(mShadowMatrix);
            DrawInhabitants(1);
            DrawRobot(1);
        glPopMatrix();
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        
        // Draw inhabitants normally
        DrawInhabitants(0);
        DrawRobot(0);

    glPopMatrix();
        
    // Do the buffer Swap
    glutSwapBuffers();
    }


void myKeyboard(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'p':
        if (timer == 0) 
        {
            timer = 1;
            glutTimerFunc(33, TimerFunction, timer);
        }
        else 
        {
            timer = 0;
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
    {
    if(key == GLUT_KEY_UP)
        frameCamera.MoveForward(0.1f);
    if(key == GLUT_KEY_DOWN)
		frameCamera.MoveForward(-0.1f);

    if(key == GLUT_KEY_LEFT)
		frameCamera.RotateLocalY(0.1f);
	   
    if(key == GLUT_KEY_RIGHT)
        frameCamera.RotateLocalY(-0.1f);
                        
    // Refresh the Window
    glutPostRedisplay();
    }

///////////////////////////////////////////////////////////
// Called by GLUT library when idle (window not being
// resized or moved)
void TimerFunction(int value)
    {
    // Redraw the scene with new coordinates
    if (value == 0) 
    {
        return;
    }
    glutPostRedisplay();
    glutTimerFunc(3,TimerFunction, timer);
    }

void ChangeSize(int w, int h)
    {
    GLfloat fAspect;

    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
        
    fAspect = (GLfloat)w / (GLfloat)h;

    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    // Set the clipping volume
    gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
        
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();    
    }

int main(int argc, char* argv[])
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800,600);
    glutCreateWindow("OpenGL SphereWorld Demo + Texture Maps");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(myKeyboard);
    buildPopupMenu();

    SetupRC();

    glutMainLoop();
    
    ShutdownRC();

    return 0;
    }
void m3dFindNormal(GLfloat result[9], const GLfloat point1[3], const GLfloat point2[3],
    const GLfloat point3[3])
{
    GLfloat v1[3], v2[3];		// Temporary vectors
    // Calculate two vectors from the three points. Assumes counter clockwise
    // winding!
    v1[0] = point1[0] - point2[0];
    v1[1] = point1[1] - point2[1];
    v1[2] = point1[2] - point2[2];

    v2[0] = point2[0] - point3[0];
    v2[1] = point2[1] - point3[1];
    v2[2] = point2[2] - point3[2];

    // Take the cross product of the two vectors to get
    // the normal vector.
   /* cout << "old" << endl;
    for (int i = 0; i < 9; i++) {
        cout << result[i] << ",";
        if (i % 3 == 2) {
            cout << endl;
        }
    }*/
    m3dCrossProduct(result, v1, v2);
    /* cout << "new" << endl;
     for (int i = 0; i < 9; i++) {
         cout << result[i] << ",";
         if (i % 3 == 2) {
             cout << endl;
         }
     }*/
    glNormal3fv(result);
}
void m3dGetPlaneEquation(GLfloat planeEq[4], const GLfloat p1[3], const GLfloat p2[3], const GLfloat p3[3])
{
    // Get two vectors... do the cross product
    GLfloat v1[3], v2[3];

    // V1 = p3 - p1
    v1[0] = p3[0] - p1[0];
    v1[1] = p3[1] - p1[1];
    v1[2] = p3[2] - p1[2];

    // V2 = P2 - p1
    v2[0] = p2[0] - p1[0];
    v2[1] = p2[1] - p1[1];
    v2[2] = p2[2] - p1[2];

    // Unit normal to plane - Not sure which is the best way here
    m3dCrossProduct4(planeEq, v1, v2);
    m3dNormalizeVector4(planeEq);
    // Back substitute to get D
    planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}
void m3dMakePlanarShadowMatrix(GLfloat proj[16], const GLfloat planeEq[4], const GLfloat vLightPos[3])
{
    // These just make the code below easier to read. They will be 
    // removed by the optimizer.	
    float a = planeEq[0];
    float b = planeEq[1];
    float c = planeEq[2];
    float d = planeEq[3];

    float dx = -vLightPos[0];
    float dy = -vLightPos[1];
    float dz = -vLightPos[2];

    // Now build the projection matrix
    proj[0] = b * dy + c * dz;
    proj[1] = -a * dy;
    proj[2] = -a * dz;
    proj[3] = 0.0;

    proj[4] = -b * dx;
    proj[5] = a * dx + c * dz;
    proj[6] = -b * dz;
    proj[7] = 0.0;

    proj[8] = -c * dx;
    proj[9] = -c * dy;
    proj[10] = a * dx + b * dy;
    proj[11] = 0.0;

    proj[12] = -d * dx;
    proj[13] = -d * dy;
    proj[14] = -d * dz;
    proj[15] = a * dx + b * dy + c * dz;
    // Shadow matrix ready
}
void m3dCrossProduct4(GLfloat result[4], const GLfloat v1[3], const GLfloat v2[3])
{
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
    result[3] = 0;
}
void m3dCrossProduct(GLfloat result[9], const GLfloat v1[3], const GLfloat v2[3])
{
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[4] = v1[2] * v2[0] - v1[0] * v2[2];
    result[8] = v1[0] * v2[1] - v1[1] * v2[0];
}
void m3dNormalizeVector(GLfloat result[9])
{
    GLfloat d = sqrt((result[0]) * (result[0]) + (result[4]) * (result[4]) + (result[8]) * (result[8]));
    result[0] /= d;
    result[4] /= d;
    result[8] /= d;
}
void m3dNormalizeVector4(GLfloat result[4])
{
    GLfloat d = sqrt((result[0]) * (result[0]) + (result[1]) * (result[1]) + (result[2]) * (result[2]) + (result[3]) * (result[3]));
    result[0] /= d;
    result[1] /= d;
    result[2] /= d;
    result[3] /= d;
}

//void move_camera(float direction) 
//{
//    
//    x = x + direction;
//
//}
void drawTorus(double r , double c ,int rSeg , int cSeg ,GLuint texture)
{
    glFrontFace(GL_CW);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    const double PI = 3.1415926535897932384626433832795;
    const double TAU = 2 * PI;

    for (int i = 0; i < rSeg; i++) {
        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                double s = (i + k) % rSeg + 0.5;
                double t = j % (cSeg + 1);

                double x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                double y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                double z = r * sin(s * TAU / rSeg);

                double u = (i + k) / (float)rSeg;
                double v = t / (float)cSeg;

                glTexCoord2d(u, v);
                glNormal3f(2 * x, 2 * y, 2 * z);
                glVertex3d(2 * x, 2 * y, 2 * z);
            }
        }
        glEnd();
    }

    glFrontFace(GL_CCW);
}
void drawLeftleg(float angle)
{
    glPushMatrix();
    {
        glTranslatef(-0.05f, -0.23f, 0.0f);
        glutSolidSphere(0.03f, 50, 50);
        //籐
        glPushMatrix();
        {
            if (anim_flag == anim_leg) {
                glRotatef(-angle * 2.0f, 1.0f, 0.0f, 0.0f);
            }
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, +0.08f, 0.0f);
            //渐籠
            glPushMatrix();
            {
                glTranslatef(0.0f, -0.12f, 0.0f);
                glutSolidSphere(0.03f, 50, 50);
                //籐
                glPushMatrix();
                {

                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, +0.08f, 0.0f);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}
void drawRightleg(float angle)
{
    glPushMatrix();
    {
        glTranslatef(+0.05f, -0.23f, 0.0f);
        glutSolidSphere(0.03f, 50, 50);
        //籐
        glPushMatrix();
        {
            if (anim_flag == anim_leg) {
                glRotatef(-angle * 2.0f, 1.0f, 0.0f, 0.0f);
            }
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, +0.08f, 0.0f);
            //渐籠
            glPushMatrix();
            {
                glTranslatef(0.0f, -0.12f, 0.0f);
                glutSolidSphere(0.03f, 50, 50);
                //籐
                glPushMatrix();
                {
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, +0.08f, 0.0f);
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}
void drawLefthand(float angle)
{
    glPushMatrix();
    {
        glTranslatef(-0.14f, 0.07f, 0.0f);
        glutSolidSphere(0.03f, 50, 50);
        //籐
        glPushMatrix();
        {
            if (anim_flag == anim_arms) {
                glRotatef(-angle -45.0f, 0.0f, 0.0f, 1.0f);
            }
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, +0.08f, 0.0f);
            //渐籠
            glPushMatrix();
            {
                glTranslatef(0.0f, -0.12f, 0.0f);
                glutSolidSphere(0.03f, 50, 50);
                //籐
                glPushMatrix();
                {
                    if (anim_flag == anim_arms) {
                        glRotatef(-angle - 45.0f, 0.0f, 0.0f, 1.0f);
                    }
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, +0.08f, 0.0f);
                    glPushMatrix();
                    {
                        glTranslatef(0.0f, -0.11f, 0.0f);
                        //glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
                        GLUquadric* quad = gluNewQuadric();
                        glEnable(GL_TEXTURE_2D);
                        gluQuadricTexture(quad, GL_TRUE);//瞶ㄧ计
                        gluSphere(quad, 0.03f, 50, 50);
                        glDisable(GL_TEXTURE_2D);
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}
void drawRighthand(float angle)
{
    glPushMatrix();
    {
        glTranslatef(0.14f, 0.07f, 0.0f);
        glutSolidSphere(0.03f, 50, 50);
        //籐
        glPushMatrix();
        {
            if (anim_flag == anim_arms) {
                glRotatef(-angle * 2.0f, 1.0f, 0.0f, 0.0f);
            }
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.03f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, -0.02f, 0.0f);
            glutSolidCube(0.03f);
            glTranslatef(0.0f, +0.08f, 0.0f);
            //渐籠
            glPushMatrix();
            {
                glTranslatef(0.0f, -0.12f, 0.0f);
                glutSolidSphere(0.03f, 50, 50);
                //籐
                glPushMatrix();
                {
                    if (anim_flag == anim_arms) {
                        glRotatef(-angle *2.0f, 1.0f, 0.0f, 0.0f);
                    }
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.03f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, -0.02f, 0.0f);
                    glutSolidCube(0.03f);
                    glTranslatef(0.0f, +0.08f, 0.0f);
                    glPushMatrix(); 
                    {
                        glTranslatef(0.0f, -0.11f, 0.0f);
                        //glBindTexture(GL_TEXTURE_2D, textureObjects[SPHERE_TEXTURE]);
                        GLUquadric* quad = gluNewQuadric();
                        glEnable(GL_TEXTURE_2D);
                        gluQuadricTexture(quad, GL_TRUE);//瞶ㄧ计
                        gluSphere(quad, 0.03f, 50, 50);
                        glDisable(GL_TEXTURE_2D);
                    }
                    glPopMatrix();
                }
                glPopMatrix();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}