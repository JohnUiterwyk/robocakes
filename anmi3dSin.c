#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

#include <math.h>

#define MAX 10

float polygon_x = -1;

void drawAxes()
{
    //Draw the axes because SERIOUSLY
    //x = red
    glColor3f(1,0,0);
    glBegin(GL_LINE);
    glVertex3f(-1, 0,0);
    glVertex3f(1, 0,0);
    glEnd();
    //y = Green
    glColor3f(0,1,0);
    glBegin(GL_LINE);
    glVertex3f(0,-1,0);
    glVertex3f(0,1,0);
    glEnd();
    //z = B
    glColor3f(0,0,1);
    glBegin(GL_LINE);
    glVertex3f(0,0,-1);
    glVertex3f(0,0,1);
    glEnd();
    glColor3f(1,1,1);
}

void display()
{
    int now_time, elapsed_time;
    static int start_time;
    float i, /*amplitude*/a = -.8, /*wavelength*/w = 1, af = 0.001, t = 0, k = 2*M_PI/w, x = 0, y = 0, z = 0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0f); /* "zoom" back 4 units */
    glRotatef(45.0f,1.0f, 0.0f, 0.0f); /* rotate to look down at 45 degrees */
    //drawAxes();


    //Draw in wireframe to aid debugging
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLE_STRIP);

    //For the animation
    now_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = now_time - start_time;
    start_time = now_time;

    //Draw 9 strips
    for(z = -2; z < 2; z = z + .1)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(x = -2; x < 2; x = x + .1)
        {
            y = a*sin((double)k*x+af*start_time);
            glVertex3f(x, y, z);
            //Won't know if this works until we have colour and lighting
            //y = a*sin((double)k*(z + .1)+af*start_time);
            glVertex3f(x, y, z + .1);
        }
        glEnd();
    }
    glutSwapBuffers();
}

void idle()
{
    polygon_x += 0.1;
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    float aspect = w / (float)h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75.0f, aspect, 0.01f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
