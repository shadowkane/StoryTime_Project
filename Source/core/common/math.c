/*
    Author: Habib
    Date: 12/11/2023
    Description: provide math tools
*/
#include "math.h"
#include <math.h>

unsigned int uGCD(int a, int b)
{
    //printf("uGCD\n");
    
    unsigned int iRetVal;

    // a and b need to be positive
    a = (a>0)? a:-a;
    b = (b>0)? b:-b;
    iRetVal = (a<b)? a:b; // take the smalest number
    iRetVal /=2;
    for(;iRetVal>0; iRetVal--)
    {
        if((a%iRetVal)==0 && (b%iRetVal)==0)
        {
            break;
        }
    }

    //printf("uGCD result = %d\n", iRetVal);
    return iRetVal;
}

// int iGCD(int a, int b)
// {
//     printf("iGCD\n");
//     // a and b need to be positive
//     a = (a>0)? a:-a;
//     b = (b>0)? b:-b;
//     // keep substruct the small number from the big number until both are equal, the result is the GCD(a, b)
//     while(a!=b)
//     {
//         if(a>b)
//         {
//             a-=b;
//         }
//         else
//         {
//             b-=a;
//         }
//     }

//     return a; // a and b are equal, return one of them
// }


int iLCM(int a, int b)
{
    //printf("iLCM\n");
    int iRetVal;
    
    // LCM(a, b) = |a*b|/GCD(a, b) "Euclidean algorithm"
    iRetVal = a*b;
    iRetVal = (iRetVal>0)? iRetVal:-iRetVal;
    iRetVal = iRetVal/uGCD(a, b);

    //printf("iLCM result = %d\n", iRetVal);
    return iRetVal;
}

bool bIsPrime(int iVal)
{
    bool bRet;
    int i;

    bRet = true;
    if(iVal>1 || iVal<-1)
    {
        for(i=2; i<iVal/2; i++)
        {
            if(iVal%i==0)
            {
                bRet = false;
            }
        }
    }
    else
    {
        bRet = false;
    }

    return bRet;
}

// bool bIsPrime_fast(int n)
// {
//     if (n == 2 || n == 3)
//         return true;

//     if (n <= 1 || n % 2 == 0 || n % 3 == 0)
//         return false;

//     for (int i = 5; i * i <= n; i += 6)
//     {
//         if (n % i == 0 || n % (i + 2) == 0)
//             return false;
//     }

//     return true;
// }