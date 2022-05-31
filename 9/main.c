#include <stdio.h>
#include "GL//glut.h"
#include "GL//glu.h"
#include "math.h"


double funnv();
const double g = 9.80665;
const double s = 0.03;
const double c = 0.6;
const double hmot = 10.0;
const double ro = 1.3;
const double dt = 0.01;

void writeA(double *vz, double *z){
    FILE *file = NULL;

    char *name = "analytically.dat";

    file = fopen(name, "a+");

    fprintf(file, "%.2lf\t", *vz);
    fprintf(file, "%.2lf\n", *z);

    fclose(file);
}
void writeN(double *vz, double *z){
    FILE *file = NULL;

    char *name = "numerically.dat";

    file = fopen(name, "a+");

    fprintf(file, "%.2lf\t", *vz);
    fprintf(file, "%.2lf\n", *z);

    fclose(file);
}

void analytically(double v0, double z0){
    double  vTerm = sqrt((2 * hmot * g) / (c * s * ro));


    if (v0 != 0)
        return;

    double t = 0, vz = 0, z = 0;
    double tFinish = sqrt((2 * z0) / g);
    printf("Time finish: %lf\n", tFinish);

    for ( t = 0; t < 3.22; t += dt) {
        vz = -vTerm * tanh((g * t) / vTerm);
        z = z0 - ( (vTerm * vTerm) / g) * log10(cosh((g * t) / vTerm));
        writeA(&vz, &z);
    }

    printf("Vterm = %lf\n", vTerm);


}



void numerically(double v0, double z0){

    double  K = 0.5 * ( (c * s * ro) / hmot);

    double t0 = 0;

    double tFinish = sqrt((2 * z0) / g);

    printf("%lf", tFinish);

    double t, vz = v0, z = z0;

//    double *t_array = calloc(1000, sizeof(double )),
//            *vz_array = calloc(1000, sizeof(double )),
//            *z_array = calloc(1000, sizeof(double ));


    for (t = 0; t < 5.09; t+=dt) {
        writeN(&vz, &z);
        if (z < 0)
            break;
        z = z + vz * dt;
        vz = vz - g * dt;
    }
}

int main() {
    double v0 = 0, z0 = 0 ;

    printf("Enter data for :\n1)Analytically:\n");
    printf("v0: ");
    scanf("%lf", &v0);

    printf("z0: ");
    scanf("%lf", &z0);

    analytically(v0, z0);

    printf("Enter data for :\nNumerically:\n");

    printf("v0: ");
    scanf("%lf", &v0);

    printf("z0: ");
    scanf("%lf", &z0);

    numerically(v0, z0);



    return 0;
}
