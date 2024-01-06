//Kevin Truong (1834510)
//ktruon13@ucsc.edu
// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"

// User libraries:
#include "MatrixMath.h"

void MatrixPrint(float mat[DIM][DIM]) {
    printf("\n _______________________ ");
    float value;
    for (int rows = 0; rows < DIM; rows++) {
        printf("\n|");
        for (int cols = 0; cols < DIM; cols++) {
            value = mat[rows][cols];
            if (value < 0) { //Checking how many digits each value have to determine space
                if (value>-10) {
                    printf("  %.2f|", value);
                } else if (value>-100) {
                    printf(" %.2f|", value);
                } else {
                    printf("%.2f|", value);
                }
            }
            if (value >= 0) {
                if (value < 10) {
                    printf("   %.2f|", value);
                } else if (value < 100) {
                    printf("  %.2f|", value);
                } else {
                    printf(" %.2f|", value);
                }
            }
        }
        printf("\n ----------------------- ");
    }
    printf("\n");
}

int MatrixEquals(float mat1[DIM][DIM], float mat2[DIM][DIM]) {
    float value1, value2;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value1 = mat1[rows][cols];
            value2 = mat2[rows][cols];
            if (!((value1 - FP_DELTA) < value2) || !((value1 + FP_DELTA) > value2)) {
                return 0;
            }
        }
    }
    return 1;
}

void MatrixAdd(float mat1[DIM][DIM], float mat2[DIM][DIM], float result[DIM][DIM]) {
    float value1, value2;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value1 = mat1[rows][cols];
            value2 = mat2[rows][cols];
            result[rows][cols] = value1 + value2;
        }
    }
}

void MatrixMultiply(float mat1[DIM][DIM], float mat2[DIM][DIM], float result[DIM][DIM]) {
    float value1, value2;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value1 = mat1[rows][cols];
            value2 = mat2[rows][cols];
            result[rows][cols] = value1*value2;
        }
    }
}

void MatrixScalarAdd(float x, float mat[DIM][DIM], float result[DIM][DIM]) {
    float value;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value = mat[rows][cols];
            result[rows][cols] = value + x;
        }
    }
}

void MatrixScalarMultiply(float x, float mat[DIM][DIM], float result[DIM][DIM]) {
    float value;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value = mat[rows][cols];
            result[rows][cols] = value * x;
        }
    }
}

float MatrixTrace(float mat[DIM][DIM]) {
    int value = 0;
    for (int x = 0; x < DIM; x++) {
        value = value + mat[x][x];
    }
    return value;
}

void MatrixTranspose(float mat[DIM][DIM], float result[DIM][DIM]) {
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            result[rows][cols] = mat[cols][rows];
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[DIM][DIM], float result[2][2]) {
    int rowc = 0;
    for (int rows = 0; rows < DIM; rows++) {
        int colc = 0;
        for (int cols = 0; cols < DIM; cols++) {
            if (i != rows && j != cols) {
                result[rowc][colc] = mat[rows][cols];
                colc++;
            }
        }
        if(colc == 2 && rowc == 0){
                rowc=1;
            }
    }
}

float MatrixDeterminant(float mat[DIM][DIM]) {
    float a = (mat[0][0])*(((mat[1][1]) * (mat[2][2]))-((mat[1][2]) * (mat[2][1]))); //Equation for 3x3 Matrix
    float b = (mat[0][1])*(((mat[1][0]) * (mat[2][2]))-((mat[1][2]) * (mat[2][0])));
    float c = (mat[0][2])*(((mat[1][0]) * (mat[2][1]))-((mat[1][1]) * (mat[2][0])));
    return a - b + c;
}

void MatrixInverse(float mat[DIM][DIM], float result[DIM][DIM]) {
    float value;
    for (int rows = 0; rows < DIM; rows++) {
        for (int cols = 0; cols < DIM; cols++) {
            value = mat[rows][cols];
            if (value != 0) {
                result[rows][cols] = 1 / value;
            } else {
                result[rows][cols] = 0;
            }
        }
    }
}

