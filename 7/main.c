#include <stdio.h>
#include "GL/glut.h"
#include "GL/glu.h"
#include "math.h"

//@author Illia Ponomarov

const double PI = 3.14f;
const double g = 9.81f;
const double dt = 0.1f;
const int step = 30;

const double nParal = 10;
const double nMerid = 10;
double time;
double rad, alfa_const, fi_const, v0_const, z0_const, v0x_const, v0y_const, v0z_const;
double xcam_min, xcam_max, ycam_min, ycam_max, zcam_min, zcam_max, dnear, dfar;

float xData[2000], yData[2000], zData[2000], x, y, z;
int i = 0;
void calculate(double h0, double v0, double alfa, double fi, double radius);
void display();
void write(float *x, float *y, float *z);
void display_2();

void video(){
    float
    vrh_xmin, vrh_xmax,
    vrh_ymin, vrh_ymax,
    vrh_zmin, vrh_zmax;

    float vrh_Lx, vrh_Ly, vrh_Lz;

    float
    scena_xmin,
    scena_ymin,
    scena_zmin;

    float
    scena_xmax,
    scena_ymax,
    scena_zmax;

    float
    scena_Lx,
    scena_Ly,
    scena_Lz;


    if(v0x_const >= 0){
        vrh_xmin = 0;;
        vrh_xmax = v0x_const * time;
    } else {
        vrh_xmin = v0x_const * time;
        vrh_xmax = 0;
    }

    if(v0y_const >= 0){
        vrh_ymin = 0;;
        vrh_ymax = v0y_const * time;
    } else {
        vrh_ymin = v0y_const * time;
        vrh_ymax = 0;
    }

    if (v0z_const > 0){
        vrh_zmin = fminf(0, z0_const);
        vrh_zmax = z0_const;
    } else{
        vrh_zmin = fminf(0, z0_const);
        vrh_zmax = z0_const + (pow(v0z_const, 2)) / 2 * g;
    }

    vrh_Lx = vrh_xmax - vrh_xmin;
    vrh_Ly = vrh_ymax - vrh_ymin;
    vrh_Lz = vrh_zmax - vrh_zmin;

    scena_xmin = vrh_xmin - 0.05 * vrh_Lx - 10 * rad;
    scena_xmax = vrh_xmax + 0.05 * vrh_Lx + 10 * rad;

    scena_ymin = vrh_ymin - 0.05 * vrh_Ly - 10 * rad;
    scena_ymax = vrh_ymax + 0.05 * vrh_Ly + 10 * rad;

    scena_zmin = vrh_zmin - 0.05 * vrh_Lz - 10 * rad;
    scena_zmax = vrh_zmax + 0.05 * vrh_Lz + 10 * rad;

    scena_Lx = scena_xmax - scena_xmin;
    scena_Ly = scena_ymax - scena_ymin;
    scena_Lz = scena_zmax - scena_zmin;

    xcam_min = scena_ymin;
    xcam_max = scena_ymax;

    ycam_min = scena_zmin;
    ycam_max = scena_zmax;

    zcam_min = scena_xmin;
    zcam_max = scena_xmax;

    dnear = -zcam_max;
    dfar = -zcam_min;

}


void update(int iStep){


    x = xData[i];
    y = yData[i];
    z = zData[i];
    i++;
    glTranslatef(x, y, z);
    glLoadIdentity();
    glutPostRedisplay();
    glutTimerFunc(step, update, iStep + 1);
}
void draw_sphere(){


    int test = 4;
    // -1.41	2.82	4.66
    float x,y,z,i,j;
    //glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    for (j = 0;j < PI; j+= PI/(nParal+1)){
        glBegin(GL_LINE_LOOP);
        y=(float) (rad  + test  * cos(j));
        for(i = 0; i < 2  * PI; i+= PI/60){
            x=(float) (rad + test * cos(i) * sin(j));
            z=(float) (rad + test * sin(i) * sin(j));
            glVertex3f(x,y,z);
        }
        glEnd();
    }


    for(j=0; j < PI; j += PI / nMerid){
        glBegin(GL_LINE_LOOP);
        for(i = 0; i < 2 * PI; i+= PI / 60){
            x=(float) (rad + test * sin(i) * cos(j));
            y=(float) (rad + test * cos(i));
            z=(float) (rad + test * sin(j) * sin(i));
            glVertex3f(x ,y , z );
        }
        glEnd();
    }


   // glPopMatrix();

}

void init() {

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, 4.0/3.0, 5, 60);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(4, 6, 5, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char** argv) {

    calculate(0, 20, 45, 90, 2);
    video();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(80, 80);
    glutInitWindowSize(1200, 1500);
    glutCreateWindow("I'm window :3");
    glutDisplayFunc(display_2);
    init();
    glutTimerFunc(step, update, 0);
    glutMainLoop();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    glColor3f(1.0, 1.0, 0.0);
    glLoadIdentity();
//    gluLookAt(10.0, 3.5, 2.0, // Положение глаз, взгляд "из"
//              -15.0, 3.5, -2.0, // Цель, взгляд "на"
//              0.0, 1.0, 0.0); // Пока игнорируем

    glOrtho(xcam_min, xcam_max, ycam_min, ycam_max, dnear, dfar);

    glutWireSphere(rad, 12, 4);

    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(x, y, z);

    glutWireSphere(rad, 12, 4);
    glBegin(GL_LINES);
        glVertex3f(0, -0.3, 0);
        glVertex3f(0, 0.3, 0);
    glEnd();

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-x, y, z);

    glutWireSphere(rad, 12, 4);
    glBegin(GL_LINES);
    glVertex3f(0, -0.3, 0);
    glVertex3f(0, 0.3, 0);
    glEnd();

    glPopMatrix();

    glFlush();
    glutSwapBuffers();
}

void display_2(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    gluLookAt(10.0, 3.5, 2.0,
              -15.0, 3.5, -2.0,
              0.0, 1.0, 0.0);

    //glOrtho(xcam_min, xcam_max, ycam_min, ycam_max, dnear, dfar);



    glPushMatrix ();
       glTranslatef (x, y, z);
        draw_sphere();
    glLoadIdentity();
    glPopMatrix ();

    glutSwapBuffers();
}


void calculate(double z0, double v0, double alfa, double fi, double radius){

    z0_const = z0, v0_const = v0, alfa_const = alfa, fi_const = fi, rad = radius;

    float timeFinish = 0.0f,
    v0x = 0.0f, v0y = 0.0f, v0z = 0.0f,
    vx = 0.0f, vy = 0.0f, vz= 0.0f,
    x = 0.0f, y = 0.0f, z = 0.0f;

    rad = radius;

    v0x = v0 * cos(alfa) * cos(fi);
    v0y = v0 * cos(alfa) * sin(fi);
    v0z = v0 * sin(alfa);

    v0x_const = v0x;
    v0y_const = v0y;
    v0z_const = v0z;

    printf("z0: %.2f\tv0:%.2f\talfa: %.2f\tfi: %.2f\tradius: %.2f\t\n", z0, v0, alfa, fi, radius);
    printf("%.2f\n", v0x);
    printf("%.2f\n", v0y);
    printf("%.2f\n", v0z);

    timeFinish =  (v0z + sqrt(pow(v0z, 2) + 2 * g * z0))/ g;
    time = timeFinish;

    printf("%f", timeFinish);
    int l = 0;

    for (float timeStart = 0.0f; timeStart <= timeFinish;) {

        x = v0x * timeStart;
        y = v0y * timeStart;
        z = z0 + v0z * timeStart - 0.5 * g * pow(timeStart, 2);
        timeStart += dt;
        xData[l] = x;
        yData[l] = y;
        zData[l] = z;
        l++;
        // printf("x: %.2f\ty: %.2f\tz: %.2f\n", x, y, z);
        write(&x, &y, &z);
    }

    for (int j = 0; j < 2000; ++j) {
        if (xData[j] != 0.0f && yData[j] != 0.0f &&zData[j] != 0.0f)
        printf("%.2f\t%.2f\t%.2f\n", xData[j], yData[j],zData[j]);
    }
}

void write(float *x, float *y, float *z){
    FILE *file = NULL;
    char* filename = "coordinates.txt";

    if ((file = fopen(filename, "a+")) == NULL)
        printf("Error\n");


    fprintf(file, "%.2f\t", *x);
    fprintf(file, "%.2f\t",*y);
    fprintf(file, "%.2f\n", *z);


    fclose(file);
}


