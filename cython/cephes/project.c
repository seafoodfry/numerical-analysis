#include <stdio.h>
#include "mconf.h"
#include "protos.h" // Ensure this includes the prototype for polevl

int main(void) {
    // Coefficients of the polynomial 2x^2 + 3x + 1
    //double coef[] = {1, 3, 2};
    double coef[] = {2, 3, 1};
    int N = 2; // Degree of the polynomial
    double x = 5.0; // Point at which to evaluate the polynomial

    // Call the polevl function
    double result = polevl(x, coef, N);

    printf("The value of the polynomial at x = %.2f is %.2f\n", x, result);

    return 0;
}

