#include <stdio.h>

#include "GL/glut.h"
#include "GL/glu.h"

const float MAX = 50.0;
const int step = 25;

float moveOn = 0.0;
float moveOnOn = 0.0;

void update_1(const int iStep){
    moveOn+=0.3;
    glutPostRedisplay();
    glutTimerFunc(step, update_1, iStep + 1);
}

void update_2(const int iStep){
    moveOnOn+=0.1;
    glutPostRedisplay();
    glutTimerFunc(step, update_2, iStep + 1);
}


void manageResize(int width, int height){
    const float sideRatio = ((float) height / width);
    float matrix[16];

    glViewport(0,0, width, height);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if (width == 0)
        width++;

    for (int i = 0; i < 16; ++i)
        matrix[i] =0.0;

    matrix[0] = 2.0 / MAX;
    matrix[5] = 2.0 / (MAX * sideRatio);
    matrix[10] = 1.0;
    matrix[15] = 1.0;


    glLoadMatrixf(matrix);


}
void triangle_1(){

    glColor3f(0,0,1);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    glScalef(2.0, 2.0, 1.0);
    glTranslatef(moveOn, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
        glVertex2f(-20.8,-1.80);
        glVertex2f(-19.8,-1.80);
        glVertex2f(-20.8,-1);
    glEnd();
}
void triangle_2(){

    glColor3f(1,1,1);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    glScalef(2.0, 2.0, 1.0);
    glTranslatef(moveOnOn, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
        glVertex2f(1,-1.8);
        glVertex2f(2.0,-1.8);
        glVertex2f(1,-1);
    glEnd();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    triangle_1();
    triangle_2();

    glutSwapBuffers();

}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(480,480);
    glutInitWindowPosition(50,50);
    glutCreateWindow(" Hello, Im a window :3 ");
    
    glutDisplayFunc(display);
    glClearColor(0.65,0,1,0);
    glutReshapeFunc(manageResize);

    glutTimerFunc(step, update_1, 0);
    glutTimerFunc(step, update_2, 0);

    glutMainLoop();

    return 0;
}