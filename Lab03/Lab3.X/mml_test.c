// **** Include libraries here ****
// Standard libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//CMPE13 Support Library
#include "BOARD.h"




// User libraries:
#include "MatrixMath.h"


// Module-level variables:
float zero_matrix[3][3] = {
    {},
    {},
    {}
};
float mat1[3][3] = {
    {1, 1, 1},
    {1, 1, 1},
    {1, 1, 1}
};
float mat2[3][3] = {
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
};
float mat3[3][3] = {
    {-1, -2, -3},
    {-4, -5, -6},
    {-7, -8, -9}
};

float mat4[3][3] = {
    {-136.07, -871.41, -917.71},
    {-403.95, -377.36, 27.30},
    {658.77, -221.02, 828.12}
};
float mat5[3][3] = {
    {(-1.0 / 1), (-1.0 / 2), (-1.0 / 3)},
    {(-1.0 / 4), (-1.0 / 5), (-1.0 / 6)},
    {(-1.0 / 7), (-1.0 / 8), (-1.0 / 9)}
};
float mat6[3][3] = {
    {1, 4, 7},
    {2, 5, 8},
    {3, 6, 9}
};

int main() {
    BOARD_Init();

    printf("Beginning ktruon13's mml test harness, compiled on %s %s\n", __DATE__, __TIME__);

    printf("Demonstrating MatrixPrint():\n");
    MatrixPrint(zero_matrix);
    MatrixPrint(mat1);
    MatrixPrint(mat2);
    MatrixPrint(mat3);
    MatrixPrint(mat4);
    MatrixPrint(mat5);


    // Add more tests here!
    int mate = 0, matm = 0, matsm = 0, matd = 0, mata = 0, matsa = 0, mati = 0, mattrans = 0, mattrace = 0, matsub = 0; //Initialize Pass Counts

    //Testing MatrixEquals()
    if (MatrixEquals(mat1, mat1)) {
        mate++;
    }
    if (!MatrixEquals(mat3, mat4)) {
        mate++;
    }

    float test[3][3]; //Initialize result test matrix

    //Testing MatrixMultiply()
    MatrixMultiply(mat4, zero_matrix, test);
    if (MatrixEquals(test, zero_matrix)) {
        matm++;
    }
    MatrixMultiply(mat4, mat1, test);
    if (MatrixEquals(test, mat4)) {
        matm++;
    }

    //Testing MatrixScalarMultiply()
    MatrixScalarMultiply(0, mat3, test);
    if (MatrixEquals(test, zero_matrix)) {
        matsm++;
    }
    MatrixScalarMultiply(1, mat3, test);
    if (MatrixEquals(test, mat3)) {
        matsm++;
    }

    //Testing MatrixDeterminant()
    if ((MatrixDeterminant(mat1)) == 0) {
        matd++;
    }
    if ((MatrixDeterminant(mat2)) == 0) {
        matd++;
    }

    //Testing MatrixAdd()
    MatrixAdd(zero_matrix, mat1, test);
    if (MatrixEquals(test, mat1)) {
        mata++;
    }
    MatrixAdd(mat3, mat2, test);
    if (MatrixEquals(test, zero_matrix)) {
        mata++;
    }

    //Testing MatrixScalarAdd()
    MatrixScalarAdd(1, zero_matrix, test);
    if (MatrixEquals(test, mat1)) {
        matsa++;
    }
    MatrixScalarAdd(-1, mat1, test);
    if (MatrixEquals(test, zero_matrix)) {
        matsa++;
    }

    //Testing MatrixInverse()
    MatrixInverse(mat3, test);
    if (MatrixEquals(test, mat5)) {
        mati++;
    }
    MatrixInverse(zero_matrix, test);
    if (MatrixEquals(test, zero_matrix)) {
        mati++;
    }

    //Testing MatrixTranspose()
    MatrixTranspose(mat6, test);
    if (MatrixEquals(test, mat2)) {
        mattrans++;
    }
    MatrixTranspose(zero_matrix, test);
    if (MatrixEquals(test, zero_matrix)) {
        mattrans++;
    }

    //Testing MatrixTrace()
    if (MatrixTrace(mat1) == 3) {
        mattrace++;
    }
    if (MatrixTrace(mat3) == -15) {
        mattrace++;
    }
    
    //Testing MatrixSubmatrix()
    float twobytwo[2][2];
    MatrixSubmatrix(1,2,mat4,twobytwo);
    if(twobytwo[0][0] == mat4[0][0]){
        if(twobytwo[0][1] == mat4[0][1]){
            if(twobytwo[1][0] == mat4[2][0]){
                if(twobytwo[1][1] == mat4[2][1]){
                    matsub++;
                }
            }
        }
    }
    MatrixSubmatrix(0,1,mat3,twobytwo);
    if(twobytwo[0][0] == mat3[1][0]){
        if(twobytwo[0][1] == mat3[1][2]){
            if(twobytwo[1][0] == mat3[2][0]){
                if(twobytwo[1][1] == mat3[2][2]){
                    matsub++;
                }
            }
        }
    }
    
    //Results from all the testing
    printf("\nPASSED (%d/2): MatrixEquals()", mate);
    printf("\nPASSED (%d/2): MatrixMultiply()", matm);
    printf("\nPASSED (%d/2): MatrixScalarMultiply()", matsm);
    printf("\nPASSED (%d/2): MatrixDeterminant()", matd);
    printf("\nPASSED (%d/2): MatrixAdd()", mata);
    printf("\nPASSED (%d/2): MatrixScalarAdd()", matsa);
    printf("\nPASSED (%d/2): MatrixInverse()", mati);
    printf("\nPASSED (%d/2): MatrixTranspose()", mattrans);
    printf("\nPASSED (%d/2): MatrixTrace()", mattrace);
    printf("\nPASSED (%d/2): MatrixSubmatrix()\n", matsub);
    BOARD_End();
    while (1);
}
