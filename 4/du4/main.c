#include <stdio.h>
#include "GL//glut.h"
#include "GL//glu.h"
#include "math.h"

int move = 0.0;
const float MAX = 50.0;
const int step = 5;
int  x =60 , y = 50 ;
float xData[2000], yData[2000], dt = 0.1;
int flag = 1;
FILE *file = NULL;
const float g = 9.81;
int j = 0;



char * filename = "C:\\Users\\hp\\CLionProjects\\fzhpt_4\\cmake-build-debug\\text.txt";

void writeFirst(float* h0, float* v0, float* alpha);
void writeSecond(float* t, float* xsur, float* ysur);
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

void update_1(const int iStep){

    if (xData[j] <= 0 && yData[j] <= 0)
        x = 0, y = 0;
    else {
        x = -xData[j];
        x = x /90;
        y = yData[j];
        y = y / 28;

        printf("%d\t%d\n", x, y);

    }
    j++;
    glutPostRedisplay();
    glutTimerFunc(step, update_1, iStep  + 1);
}

void display(void) {

        float angle_theta;
        glColor3f(0, 0.5, 1);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

    gluOrtho2D(0.0, 800.0,
               0.0, 600.0);

        if (flag) {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; i++) {
                angle_theta = i * 3.142 / 180;
                glVertex2f(x + 50 * cos(angle_theta),
                           y + 50 * sin(angle_theta));
            }
            glEnd();
        }
        glutSwapBuffers();

}



int main(int argc, char** argv){
    float h0 = 0, v0 = 0, alpha = 0, xsur = 0, ysur = 0, y0 = 50, vX = 0, vY = 0, v = 0;
    writeFirst(&h0, &v0, &alpha);

    float timeFinish = (2 * v0 * sin(alpha)) / g;

    file = fopen(filename, "a+");


    vY = v0 * sin(alpha);
    vX = v0 * cos(alpha);

    printf("%.2f\t", y0);
    printf("%.2f\t", v0);
    printf("%.2f\t", alpha);
    printf("\n");

int i = 0;
    for (float timeStart = 0; timeStart <= timeFinish ; timeStart += dt) {
        xsur = xsur + vX * dt;
        ysur = ysur + vY * dt;
        vY = v0 * sin(alpha) - g * timeStart;
        v = sqrt(pow(vX, 2) + pow(vY, 2));

        fprintf(file, "%.2f\t", xsur);
        fprintf(file, "%.2f\t", ysur);
        fprintf(file, "\n");
        xData[i] = xsur;
        yData[i] = ysur;
//        printf("%.2f\t", timeStart);
//        printf("%.2f\t", xsur);
//        printf("%.2f\t", ysur);
//        printf("%.2f\t", vX);
//        printf("%.2f\t", vY);
//        printf("%.2f\t", v);
//        printf("\n");
        i++;

    }

   /* for (int k = 0; k < sizeof (xData) / sizeof (float); ++k) {
        printf("%f\t%f\n", xData[k], yData[k]);
    }*/

    glutInit(&argc, argv);
    glutInitWindowSize(2500, 1800);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow("Help me <3");

    glClearColor(0, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(display);
    glutReshapeFunc(manageResize);

    j = 0;
    glutTimerFunc(step, update_1, 0);
    glutMainLoop();
}



void writeFirst(float* h0, float* v0, float* alpha){


    printf("Height: ");
    scanf("%f", &(*h0));

    while (*h0 < 0)
        scanf("%f", &(*h0));



    printf("Speed: ");
    scanf("%f", &(*v0));

    printf("Alpha: ");
    scanf("%f", &(*alpha));

    if ((file = fopen(filename, "a+")) == NULL) {
        printf("Error occured while opening text.txt");
        exit(0);
    }




    fclose(file);
}
void writeSecond(float* t, float* xsur, float* ysur) {

}
