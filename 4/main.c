#include <stdio.h>
#include "GL//glut.h"
#include "GL//glu.h"
#include "math.h"

const float MAX = 50.0;
const int step = 1;
int  x =60 , y = 50 ;
float xData[2000], yData[2000], dt = 0.1, sizeX, sizeY;
float xBuffer[2000], yBuffer[2000];
float xMax, yMax;

float al ;
int flag = 1;
FILE *file = NULL;
const float g = 9.81;
int j = 0;



char * filename = "coordinates.txt";

void quicksort(float number[2000],int first,int last);
void writeFirst(float* h0, float* v0, float* alpha);
void writeMax();
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
void update_1(const int iStep) {

    if (al == 90) {
        while (1)
            y += 1;
    } else if (al == -90) {
        while (1) {
            y -= 1;

        }

    }

    if (xData[j] <= 0 && yData[j] <= 0)
        x = 0, y = 0;
    else {
        x = -xData[j];
        x = x / 90;
        y = yData[j];
        y = y / 28;

        if (y == 0) {
            printf("stop.");
            exit(0);
        }
        printf("%d\t%d\n", x, y);
    }
    j++;
    glutPostRedisplay();
    glutTimerFunc(step, update_1, iStep + 1);
}
void create(){
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

    int i = 0;float xf, yf;
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
        xf = -xData[j];
        xf = xf / 90;
        yf = yData[j];
        yf = yf / 28;
        xBuffer[j] = xf;
        yBuffer[j] = yf;
        i++;

    }
    fclose(file);

    int size = sizeof(xBuffer) / sizeof(xBuffer[0]);

    for (int k = 0; k < size; ++k) {
        xBuffer[k] = -xData[k];
        xBuffer[k] =xBuffer[k] / 90;

        yBuffer[k] = yData[k];
        yBuffer[k] /= 28;

    }

    quicksort(xBuffer, 0, size - 1);
    quicksort(yBuffer, 0, size - 1);




    xMax = xBuffer[size - 1];
    yMax = yBuffer[size - 1];

    writeMax();

    printf("xMax: %f\tyMax:%f\n", xMax, yMax);


}

void writeMax(){
    char* name = "yxMax.txt";

    FILE* fl = NULL;

    if ((fl = fopen(name, "w+")) == NULL) {
        printf("problem");
        exit(0);
    }else{
        printf("%f\t%\n", xMax, yMax);
        fprintf(fl, "%.2f\t", xMax);
        fprintf(fl, "%.2f\n", yMax);
    }
    fclose(fl);
}


void  display_circle() {

    float angle_theta;
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0.5, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(20.0, -1920,
               0.0, 1080);

    // glTranslated(-1920, 1080, 0);

    gluLookAt(0, 0, -1,
              0, 0, 0,
              0, 1, 0);



    // glTranslated(-1920, 1080, 0);



    if (flag) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; i++) {
            angle_theta = i * 3.142 / 180;
            glVertex2f(x + sizeX * cosf(angle_theta),
                       y + sizeY * sin(angle_theta));
        }
        glEnd();
    }
    glutSwapBuffers();

}
void quicksort(float number[25],int first,int last){
    int i, j, pivot, temp;

    if(first<last){
        pivot=first;
        i=first;
        j=last;

        while(i<j){
            while(number[i]<=number[pivot]&&i<last)
                i++;
            while(number[j]>number[pivot])
                j--;
            if(i<j){
                temp=number[i];
                number[i]=number[j];
                number[j]=temp;
            }
        }

        temp=number[pivot];
        number[pivot]=number[j];
        number[j]=temp;
        quicksort(number,first,j-1);
        quicksort(number,j+1,last);

    }
}


int main(int argc, char **argv) {

    float sx, sy;
    int choose = 0;

    create();
    glutInit(&argc, argv);

    glutInitWindowSize(1980, 1080);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow("Help me <3");
    glClearColor(0, 0, 0, 0);

    glClear(GL_COLOR_BUFFER_BIT);

            printf("Enter Height of figure: ");
            scanf("%f", &sx);

            printf("Enter Width of figure: ");
            scanf("%f", &sy);

            sizeX = sx;
            sizeY = sy;

            glutDisplayFunc(display_circle);
            glutReshapeFunc(manageResize);
            j = 0;
            glutTimerFunc(step, update_1, 0);
            glutMainLoop();
    }





void writeFirst(float* h0, float* v0, float* alpha){


    printf("Height: ");
    scanf("%f", &(*h0));

    while (*h0 < 0) {
        printf("STOP: Initial coordinate must be\n"
               "positive!\nEnter: ");
        scanf("%f", &(*h0));
    }


    printf("Speed: ");
    scanf("%f", &(*v0));

    printf("Alpha: ");
    scanf("%f", &(*alpha));

    while (*alpha < -90 || *alpha > 90){
        printf("STOP:\n"
               "Angle must be between <-90st, 90st>!\nEnter: ");
        scanf("%f", &(*alpha));
    }

    if ((file = fopen(filename, "a+")) == NULL) {
        printf("Error occured while opening text.txt");
        exit(0);
    }




    fclose(file);
}