#include <stdio.h>
#include "GL//glut.h"
#include "GL//glu.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>


#define countOfCricle 1
#define dynamic 2
#define randomPosition  (-1.0 + 2*(double)rand()/RAND_MAX)
int  count = 0;
double tableSize[1 + dynamic];
double move, yAlfa;  // pre kozmicky biliard
double coordinates[1 + countOfCricle][1 + dynamic];
double speed[1 + countOfCricle][1 + dynamic];

double x = 0.0, y = 0.0 ;


const double POLOMER = 0.10;
const double EXTRAD = 0.11;

const int step = 15;
const double dt = 0.015;
double L;

const double PI = 3.14159265359;

FILE *file = NULL;

void drawObjects2d();
void drawObjects3d();
void resize(int width, int height);
void update(const int iusek);




double dalsia_zrazka(const double PosNow[1 + countOfCricle][1 + dynamic],
                     const double VelNow[1 + countOfCricle][1 + dynamic],
                     int *OdrGula, int *OdrCart, int *igzr, int *jgzr){

    const double NepatrneKladneCislo = 1.0e20 * DBL_MIN;
    double CasMin = DBL_MAX;


    *OdrGula = 0;
    *OdrCart = -1;
    for (int ig = 1; ig <= countOfCricle; ig++)
        for (int ic = 1; ic <= dynamic; ic++) {


            double time;

            if(VelNow[ig][ic] < 0.0)

            {

                time = (PosNow[ig][ic] - (-0.5 * tableSize[ic] + POLOMER) ) / ( -VelNow[ig][ic]);

                if (time < 0.0)
                    time *= -1.0;

                if (time < CasMin) {
                    CasMin = time;
                    *OdrGula = ig;
                    *OdrCart = ic;
                }
            }
            else if (VelNow[ig][ic] >  0.0) {


                time = (0.5 * tableSize[ic] - POLOMER - PosNow[ig][ic]) / VelNow[ig][ic];

                if (time < 0.0)
                    time *= -1.0;


                if (time < CasMin) {
                    CasMin = time;
                    *OdrGula = ig;
                    *OdrCart = ic;
                }

                x = PosNow[ig][1];
                y = PosNow[ig][2];

            }
        }


    *igzr = 0;
    *jgzr = 0;

    for (int ig = 2; ig <= countOfCricle; ig++)
        for (int jg = 1; jg < ig; jg++) {

            double bij = 0.0;
            for (int ic = 1; ic <= dynamic; ic++)
                bij += (PosNow[jg][ic]-PosNow[ig][ic])*(VelNow[jg][ic]-VelNow[ig][ic]);

            double rijsq = 0.0;

            for (int ic = 1; ic <= dynamic; ic++)
                rijsq += pow((PosNow[jg][ic] - PosNow[ig][ic]), 2);

            double vijsq = 0.0;

            for (int ic = 1; ic <= dynamic; ic++)
                vijsq += pow((VelNow[jg][ig] - VelNow[ig][ic]), 2);

            if (bij < 0.0  &&  bij * bij - vijsq * (rijsq - 4 * POLOMER * POLOMER) > 0.0) {

                double tij = (-bij - sqrt(bij * bij - vijsq * (rijsq - 4 * POLOMER * POLOMER))) / vijsq;

                if (tij >= 0.0 && tij < CasMin) {
                    CasMin = tij;
                    *igzr = ig;
                    *jgzr = jg;
                    *OdrCart = 0;
                }
            }
        }
    return CasMin;
}


int main(int argc, char** argv) {


    if (dynamic < 2 || dynamic > 3) {

        fprintf(stderr, " CHYBA:  dynamic = %d je nespravne alebo nie uplne implementovane.\n\n", dynamic);
        exit(1);
    }
    if (EXTRAD <= POLOMER) {
        fprintf(stderr, " CHYBA:  extrad = %1.10lf  je prilis male. Musi byt aspon %1.10lf. Koncim.\n", EXTRAD, POLOMER);
        exit(1);
    }


    tableSize[1] = 2.84;  // dlzka stola
    tableSize[2] = 1.42;  // sirka stola
    if (dynamic == 3) {
        tableSize[3] = 2.00;  // vyska ,stola', pre nas vizualne hlbka
        move = -1.50;  // Za obrazovkou je z < 0 .
        yAlfa = 120.0;
    }

    for (int ig = 1; ig <= countOfCricle; ig++)
        for (int ic = 1; ic <= dynamic; ic++)
            speed[ig][ic] = 1;
    {
        const int ig = 1;
        for (int ic = 1; ic <= dynamic; ic++)
            coordinates[ig][ic] = randomPosition * (0.5 * tableSize[ic] - EXTRAD);
    }
    for (int ig = 2; ig <= countOfCricle; ig++) {
        bool prekryv;
        do {

            for (int ic = 1; ic <= dynamic; ic++)
                coordinates[ig][ic] = randomPosition * (0.5 * tableSize[ic] - EXTRAD);

            prekryv = false;
            for (int jg = 1; jg < ig; jg++) {
                double distsq = 0.0;
                for (int ic = 1; ic <= dynamic; ic++) distsq = pow(coordinates[ig][ic] - coordinates[jg][ic], 2);
                if (sqrt(distsq) <= 2 * EXTRAD) {
                    prekryv = true;
                    break;
                }
            }
        } while (prekryv);
    }

    L = fmax(tableSize[1], tableSize[2]);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(1250, 750);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("OpenGL: biliard");
    glClearColor(1.0, 1.0, 1.0, 0.3);

    if (dynamic <= 2)
        glutDisplayFunc(drawObjects2d);
    else
        glutDisplayFunc(drawObjects3d);


    glutReshapeFunc(resize);


     glutTimerFunc(step, update, 1);

    glutMainLoop();
    return 0;
}

void resize(int width, int height){

    const double pomstr = ((double)  height) / width;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width == 0) width++;
    if (height == 0) height++;


    if (dynamic <= 2)
        gluOrtho2D(-0.6 * L, 0.6 * L, -0.6 * L * pomstr, 0.6 * L * pomstr);
    else{
        const double invpomstr = ((double ) width) / height;
        const double dnear = 0.0 * tableSize[3];
        const double dfar = 1.0 * tableSize[3];

        gluPerspective(yAlfa, invpomstr, dnear, dfar);
    }

}

void drawObjects2d(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();
    glColor3f(0.0, 1.0, 0.0);
    glScalef(tableSize[1], tableSize[2], 1.0);
    glRectf(-0.5, -0.5, 0.5, 0.5);


    const int Plcov = 36;
    float theta;

    for (int ig = 1; ig <= countOfCricle ; ++ig) {
        glColor3f(1- (float)(ig-1) / countOfCricle, 0.0, (float)(ig - 1) / countOfCricle);
        glLoadIdentity();
        glTranslatef(coordinates[ig][1], coordinates[ig][2], 0.0);
        glScalef(POLOMER, POLOMER, 1.0);
        glBegin(GL_TRIANGLE_FAN);


        GLfloat doublePi = 2.0 * PI;

        for(int i = 0; i < 360;i++) {

            theta = i * PI / 180;
            glVertex2f(
                    coordinates[ig][1] + POLOMER * cos(theta),
                    coordinates[ig][2] + POLOMER * sin(theta)

            );
        }

        glEnd();
    }


    glutSwapBuffers();

}

void drawObjects3d(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);


    const int PLucov = 36;
    float theta;
    for (int ig = 1; ig <= countOfCricle; ig++) {
        glColor3f(1 - (float) (ig-1) / countOfCricle, 0.0, (float) (ig - 1) / countOfCricle);
        glLoadIdentity();
        glTranslatef(coordinates[ig][1], coordinates[ig][2], coordinates[ig][3] + move);
        glScalef(POLOMER, POLOMER, 1.0); // potom glScalef.
        glBegin(GL_TRIANGLE_FAN);
        GLfloat doublePi = 2.0 * PI;

        for(int i = 0; i < 360; i++) {

            theta = i * PI / 180;
            glVertex2f(
                    coordinates[i][1] + POLOMER * cos(theta),
                    coordinates[i][2] + POLOMER * sin(theta)

            );
        }

        glEnd();
    }
    glutSwapBuffers();
}
double skalsuc(const double vec1[1 + dynamic], const double vec2[1 + dynamic])
{
    double suma = 0.0;

    for (int cart = 1; cart <= dynamic; cart++)
        suma += vec1[cart]*vec2[cart];

    return suma;
}


void update(const int iusek){

    file = fopen("text.txt", "a+");
    static int odrCricle = 0, odrCart = 0, igzr = 0, jgzr = 0, PUsekov = 0;
    static double dtloc = 0.0;
    const double smallPositiveNumber = 100 * DBL_EPSILON;

    double bCa = 0.0;

    if (iusek == 1) {
        double CasZr = dalsia_zrazka(coordinates, speed, &odrCricle, &odrCart, &igzr, &jgzr);
        bCa = CasZr;
        PUsekov = 1 + (int)(CasZr/dt);
        dtloc = CasZr/PUsekov;

    }
    for (int ig = 1; ig <= countOfCricle; ig++) for (int ic = 1; ic <= dynamic; ic++)
            coordinates[ig][ic] += speed[ig][ic] * dtloc;


    glutPostRedisplay();
    if (iusek == PUsekov) {


        if (odrCart > 0) {
            speed[odrCricle][odrCart] *= -1;
            glutTimerFunc(step, update, 1);
        }
        else if (odrCart == 0) {

            double uij[1 + dynamic], vij[1 + dynamic], dvi[1 + dynamic];
            for (int ic = 1; ic <= dynamic; ic++) {
                uij[ic] = coordinates[jgzr][ic] - coordinates[igzr][ic];
            }

                double rijsq = skalsuc(uij, uij);  // Toto je naozaj  rij^2  a ma to byt rovne (2*polomer)^2.



            for (int ic = 1; ic <= dynamic; ic++) uij[ic] /= (2 * POLOMER);
            for (int ic = 1; ic <= dynamic; ic++) vij[ic] = speed[jgzr][ic] - speed[igzr][ic];
            for (int ic = 1; ic <= dynamic; ic++) dvi[ic] = uij[ic] * skalsuc(uij, vij);
            for (int ic = 1; ic <= dynamic; ic++) speed[igzr][ic] += vij[ic];
            for (int ic = 1; ic <= dynamic; ic++) speed[jgzr][ic] -= vij[ic];
            glutTimerFunc(step, update, 1);
        }
        else {
            fprintf(stderr, " aktualizuj(): Chyba: Zrazka nebola spravne urcena. Koncim.\n\n");
            exit(9);
        }
    }
    else
        glutTimerFunc(step, update, iusek + 1);



    fclose(file);
}