#ifndef CORE_TOOLS_MATH_H_
#define CORE_TOOLS_MATH_H_

#include <stdio.h>
#include <stdbool.h>

/*!
    @function   uGCD
    @brief      GCD (greatest common divisor) function
    @param      a  a number
    @param      b  a number
    @result     return GCD
*/
unsigned int uGCD(int a, int b);
//int iGCD(int a, int b); // other method to find GCD

/*!
    @function   iLCM
    @brief      LCM (Least Common Multiple)
    @param      a  a number
    @param      b  a number
    @result     return LCM
*/
int iLCM(int a, int b);

/*!
    @function   bIsPrime
    @brief      Check in value is a prime number
    @param      iVal  a number
    @result     return true if it's a prime number or false if it's not
*/
bool bIsPrime(int iVal);
//bool bIsPrime_fast(int n); // fast prime number check algorithm (src: https://en.wikipedia.org/wiki/Primality_test)

#endif //CORE_TOOLS_MATH_H_