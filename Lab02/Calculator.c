// Kevin Truong ktruon13@ucsc.edu
// Standard libraries
#include <stdio.h>
#include <math.h>


//User libraries:
#include "BOARD.h"

#ifdef PIC32
// Microchip libraries
#include <xc.h>
#endif


// **** Declare function prototypes ****
double Add(double operand1, double operand2);
double Subtract(double operand1, double operand2);
double Multiply(double operand1, double operand2);
double Divide(double operand1, double operand2);
double AbsoluteValue(double operand);
double FahrenheitToCelsius(double operand);
double CelsiusToFahrenheit(double operand);
double Average(double operand1, double operand2);
double Tangent(double operand);
double Round(double operand);

void CalculatorRun(void) {
    printf("\n\nWelcome to Kevin Truong's calculator program! Compiled at %s %s\n", __DATE__, __TIME__); //Greeting message

    while (1) {
        printf("\nEnter a mathematical operation to perform (*,/,+,-,m,a,c,f,t,r): ");
        char op; //initialize operator
        scanf(" %s", &op); //Save input of user
        if (op == '*' || op == '/' || op == '+' || op == '-' || op == 'm' || op == 'a' || op == 'c' || op == 'f' || op == 't' || op == 'r') { //Check if user input is valid
            op = op;
        } else if (op == 'q') { //Exit program
            return;
        } else if (op != '*' && op != '/' && op != '+' && op != '-' && op != 'm' && op != 'a' && op != 'c' && op != 'f' && op != 't' && op != 'r') {
            op = '0';
        }
        if (op != '0') { //Check if operator was valid
            double result;
            printf("\tEnter the first operand: ");
            double oper1; //Operand1
            double oper2; //Operand2
            scanf(" %lf", &oper1);
            if (op == '*' || op == '/' || op == '+' || op == '-' || op == 'm') { //Check to see if binary
                printf("\tEnter the second operand: ");
                scanf(" %lf", &oper2); //Get Operand2
            }
            if (op == '+') { //Call Add()
                result = Add(oper1, oper2);
                printf("\tResult of (%lf + %lf): %lf", oper1, oper2, result);
            }
            if (op == '-') { //Call Subtract()
                result = Subtract(oper1, oper2);
                printf("\tResult of (%lf - %lf): %lf", oper1, oper2, result);
            }
            if (op == '*') { //Call Multiply()
                result = Multiply(oper1, oper2);
                printf("\tResult of (%lf * %lf): %lf", oper1, oper2, result);
            }
            if (op == '/') { //Call Divide()
                if (oper2 != 0) {
                    result = Divide(oper1, oper2);
                    printf("\tResult of (%lf / %lf): %lf", oper1, oper2, result);
                } else if (oper2 == 0) {
                    printf("\tDivide by zero error!");
                }
            }
            if (op == 'm') { //Call Average()
                result = Average(oper1, oper2);
                printf("\tResult of avg(%lf, %lf): %lf", oper1, oper2, result);
            }
            if (op == 'a') { //Call Absolute Value()
                result = AbsoluteValue(oper1);
                printf("\tResult of |%lf|: %lf", oper1, result);
            }
            if (op == 'c') { //Call FahrenheitToCelsius()
                result = FahrenheitToCelsius(oper1);
                printf("\tResult of (%lf deg->C): %lf", oper1, result);
            }
            if (op == 'f') { //Call CelsiusToFahrenheit()
                result = CelsiusToFahrenheit(oper1);
                printf("\tResult of (%lf deg->F): %lf", oper1, result);
            }
            if (op == 't') { //Call Tangent()
                result = Tangent(oper1);
                printf("\tResult of tan(%lf): %lf", oper1, result);
            }
            if (op == 'r') { //Call Round()
                result = Round(oper1);
                printf("\tResult of round(%lf): %lf", oper1, result);
            }
        } else if (op == '0') { //Print error message if operator not valid
            printf("\tError not a standard operator.");
        }
    }
    BOARD_End();
    while (1);
}

/********************************************************************************
 * Define the Add function here.
 ********************************************************************************/
double Add(double operand1, double operand2) {
    return operand1 + operand2;
}

/********************************************************************************
 * Define the Subtract function here.
 ********************************************************************************/
double Subtract(double operand1, double operand2) {
    return operand1 - operand2;
}

/********************************************************************************
 * Define the Multiply function here.
 ********************************************************************************/
double Multiply(double operand1, double operand2) {
    return operand1*operand2;
}

/********************************************************************************
 * Define the Divide function here.
 ********************************************************************************/
double Divide(double operand1, double operand2) {
    return operand1 / operand2;
}

/********************************************************************************
 * Define the Absolute Value function here.
 ********************************************************************************/
double AbsoluteValue(double operand) {
    if (operand < 0) { //Check if negative
        return operand * (-1); //Return negative to positive
    }
    return operand; //Otherwise print value
}

/*********************************************************************************
 * Define the Fahrenheit to Celsius function here.
 ********************************************************************************/
double FahrenheitToCelsius(double operand) {
    return 5.0 / 9.0 * (operand - 32);
}

/*********************************************************************************
 * Define the Celsius to Fahrenheit function here.
 ********************************************************************************/
double CelsiusToFahrenheit(double operand) {
    return 9.0 / 5.0 * operand + 32;
}

/********************************************************************************
 * Define the Average function here.
 *******************************************************************************/
double Average(double operand1, double operand2) {
    return (operand1 + operand2) / 2;
}

/*********************************************************************************
 * Define the Tangent function that takes input in degrees.
 ********************************************************************************/
double Tangent(double operand) {
    return tan(operand * (M_PI / 180));
}

/*********************************************************************************
 * Define the Round function here.
 * In order to receive the extra credit your calculator MUST ALSO CORRECTLY utilize
 * this function.
 ********************************************************************************/
double Round(double operand) {
    int integer; //Will be used for comparison for rounding
    if (operand >= 0) { //Check operand if positive
        integer = operand + 1;
        if (operand + 0.5 >= integer) {
            return integer;
        }
    }
    if (operand < 0) { //Check operand if negative
        integer = operand - 1;
        if (operand - 0.5 <= integer) {
            return integer;
        }
    }
    return (int) operand; //If it can't be rounded, return value
}


