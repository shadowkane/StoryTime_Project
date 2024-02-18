#include "test_header.h"
#include "math.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define VALIDE_GCD_VALUE_1  585
#define VALIDE_GCD_VALUE_2  30
#define VALIDE_GCD_RESULT   15

#define VALIDE_LCM__VALUE_1 5689
#define VALIDE_LCM__VALUE_2 4712
#define VALIDE_LCM_RESULT   26806568

#define VALIDE_PRIME_VALUE      56813
#define VALIDE_IS_PRIME_RESULT  true

int test_math()
{
    unsigned int uGcdResult;
    int iLcmResult;
    bool bIsPrimeResult;

    //printf("------------- Start math.c Test -------------\n");

    // Test uGCD(...)
    uGcdResult = uGCD(VALIDE_GCD_VALUE_1, VALIDE_GCD_VALUE_2);
    assert(uGcdResult == VALIDE_GCD_RESULT);
    printf("test_math, test function: uGCD(...) ====> PASS\n");
    
    // Test iLCM(...)
    iLcmResult = iLCM(VALIDE_LCM__VALUE_1, VALIDE_LCM__VALUE_2);
    assert(iLcmResult == VALIDE_LCM_RESULT);
    printf("test_math, test function: iLCM(...) ====> PASS\n");

    // Test bIsPrime(...)
    bIsPrimeResult = bIsPrime(VALIDE_PRIME_VALUE);
    assert(bIsPrimeResult == VALIDE_IS_PRIME_RESULT);
    printf("test_math, test function: bIsPrime(...) ====> PASS\n");

    //printf("------------- END math.c Test -------------\n");

    return 0;
}