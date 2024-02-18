/*
    Author: Habib
    Date: 12/11/2023
    Description: provide RSA encryption and decryption tools
    
    How RSA encryption works:
        RSA (Rivest–Shamir–Adleman) cryptosystem:
        (m^e)^d = m mod(n) , and 0<= m <n
        encryption => c = m^e mod(n)
        decription => m = c^d mod(n)
        m : message to send
        c : cipher text or crypted message
        e : encryption key
        d : decryption key
        n : modulus

*/

#include "encryption_rsa.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "math.h"

xEncryptionRsaModule_t xRsaKeyGenerator(uint16_t u16MinValue, uint16_t u16MaxValue, uint16_t u16RandomValue)
{
    xEncryptionRsaModule_t xRsaModule = {0, 0, 0};
    int iVar;
    //
    uint16_t u16RandomLowerValue;
    uint16_t u16RandomUpperValue;
    uint16_t u16Prime1;
    uint16_t u16Prime1Prime2Gap;    // random gap (or difference) between prime number 1 and prime number 2
    uint16_t u16Prime2;
    uint32_t u32Modulus_CarmichaelFunction;
    uint32_t u32DecryptionKey;
    //uint32_t u32Modulus_CarmichaelFunction_CoprimeValue;
    uint16_t u16EncryptionKey;
    uint32_t u32Modulus;

    /* -step 1- generate 2 prime numbers */
    // set random generator seed
    srand(time(NULL));
    // set random values for u16RandomUpperValue and u16RandomLowerValue from u16MinValue and u16MaxValue
    // we need to keep in mind that:
    //      + u16MinValue < u16MaxValue
    //      + u16RandomUpperValue should be <= 0xFFFF
    //      + u16RandomLowerValue should be > u16MinValue
    if(u16MinValue<u16MaxValue){
        // to set a u16RandomLowerValue in a safe way, we need to add a numer to u16RandomLowerValue, in which that number is between 1 and (0xffff - u16MaxValue) to make sure u16RandomLowerValue<=0xFFFF
        // random value we will add to both u16RandomLowerValue and u16RandomUpperValue
        //iVar = (rand() % (0xFFFF-u16MaxValue)) + 1; // not optimized instruction : (rand() % ((0xFFFF-u16MaxValue) -1 +1)) + 1;
        iVar = (rand() % (0x02FF-u16MaxValue)) + 1; // a very large number can slow the encryption and the decryption, to avoid reach a very big number we lower max range 0xFFFF to 0x02FF
        #if(DEBUG>2)
        printf("Random var: %d\n", iVar);
        #endif
        u16RandomUpperValue = u16MaxValue + (uint16_t)iVar;
        u16RandomLowerValue = u16MinValue + (uint16_t)iVar;
    }
    else{
        // to avoid returning error message we will use a pre-set u16RandomUpperValue and u16RandomLowerValue
        u16RandomLowerValue = 0x100;  // since most of messages are ASCII which mean it's between 0x00 and 0xFF so the lowest value shoudl be more 0xFF
        u16RandomUpperValue = 0x200;
    }
    
    #if(DEBUG>2)
    printf("Passed lower value: %d\n", u16MinValue);
    printf("new lower value: %d\n", u16RandomLowerValue);
    printf("Passed upper value: %d\n", u16MaxValue);
    printf("new upper value: %d\n", u16RandomUpperValue);
    printf("Passed Random value: %d\n", u16RandomValue);
    #endif

    // get random value between u16RandomUpperValue-u16RandomLowerValue
    u16Prime1 = (rand() % (u16RandomUpperValue - u16RandomLowerValue + 1)) + u16RandomLowerValue;
    do
    {
        u16Prime1++;
    } while (!bIsPrime(u16Prime1));
    #if(DEBUG>2)
    printf("Random value 1: %d\n", u16Prime1);
    #endif
    // u16RandomValue is the gap between u16Prime2 and u16Prime1 and we need to make sure u16Prime2 <= 0xFFFF
    if((u16RandomValue>0) && (u16RandomValue<=(0xFFFF-u16Prime1))){
        u16Prime1Prime2Gap = u16RandomValue;
    }
    else{
        // just a random gap to avoid error
        //u16Prime1Prime2Gap = 0xFFFF-u16Prime1; 
        u16Prime1Prime2Gap = 73;
    }
    
    u16Prime2 = u16Prime1 + u16Prime1Prime2Gap;
    do
    {
        u16Prime2++;
    } while (!bIsPrime(u16Prime2));
    #if(DEBUG>2)
    printf("Random value 2: %d\n", u16Prime2);
    #endif
   /*
    u16Prime2 = (rand() % (u16RandomUpperValue - u16RandomLowerValue + 1)) + u16RandomLowerValue;
    do
    {
        u16Prime2++;
    } while (!bIsPrime(u16Prime2));
    //u16Prime2 = 53;
    printf("Random value 2: %d\n", u16Prime2);
    u16Prime1 = u16Prime2 - 73;
    do
    {
        u16Prime1++;
    } while (!bIsPrime(u16Prime1));
    //u16Prime1 = 61;
    printf("Random value 2: %d\n", u16Prime1);
    */
   
    /* -step 2- compute modulus */
    // modulus = prime number 1 * prime number 2
    u32Modulus = u16Prime1 * u16Prime2;
    #if(DEBUG>1)
    printf("Modulus : %d\n", u32Modulus);
    #endif
    /* -step 3- compute the Carmichael function λ(u32Modulus)*/
    // Since u32Modulus = u16Prime1 * u16Prime2 => λ(u32Modulus) = lcm(λ(u16Prime1), λ(u16Prime2))
    // and since u16Prime1 and u16Prime1 are prime, λ(u16Prime1) = u16Prime1 − 1, and λ(u16Prime2) = u16Prime2 − 1
    // so λ(u32Modulus) = lcm(u16Prime1 − 1, u16Prime2 − 1)
    u32Modulus_CarmichaelFunction = iLCM(u16Prime1-1, u16Prime2-1);
    #if(DEBUG>2)
    printf("Carmichael's Function at Modulus %d : %d\n", u32Modulus, u32Modulus_CarmichaelFunction);
    #endif
    /* -step 4- choose encryption key (public key exponent 'e')*/
    /*
    // The encryption key (or e) must have those criteria:
    //  1)   2 < e < λ(u32Modulus)
    //  2)   e and λ(u32Modulus) are coprime, which mean gcd(e, λ(u32Modulus)) = 1
    // note: for an efficient encryption, we should have short bit-lenght (like 16 bits) and small hamming weight (in other word, the binary representation of that value have too much zeros)
    //       for the sake of this simple application, i will make a simple encryption using only a short bit-length (16) (i will not focus on the small hamming weight)
    */
    // set e to a random number of 16-bit max and make sure it's >2
    u16EncryptionKey = (rand() % (0xFFFF - 4)) + 3;
    // also make sure e <λ(u32Modulus)
    u16EncryptionKey = (u16EncryptionKey>u32Modulus_CarmichaelFunction)? u32Modulus_CarmichaelFunction-1:u16EncryptionKey;
    #if(DEBUG>2)
    printf("initial i16EncryptionKe: %d\n", u16EncryptionKey);
    #endif
    // find e, where e and λ(u32Modulus) are coprime using GCD(e, λ(u32Modulus)) = 1 
    while((uGCD((int)u16EncryptionKey, u32Modulus_CarmichaelFunction)!=1) && (u16EncryptionKey>2))
    {
        u16EncryptionKey++;
    }
    if(u16EncryptionKey<=2)
    {
        #if(DEBUG>0)
        printf("Failed to get the encryption key\n");
        #endif
        return xRsaModule; // -TODO- avoid hamburger functions style
    }
    #if(DEBUG>1)
    printf("i16EncryptionKey: %d\n", u16EncryptionKey);
    #endif
    /* -step 5- determine decryption key (prived key exponent 'd')*/
    /*
    //  solve for d ≡ e^(−1) (mod(λ(u32Modulus))) => d*e = 1 * mod(λ(u32Modulus)) (d is the multiplicative inverse of e modulo λ(u32Modulus) and it's exist because e and λ(u32Modulus) are coprime, GCD(e, λ(u32Modulus))=1 )
    //  using Extended Euclidean algorithm: d*e = 1 mod(λ(u32Modulus)) => d*e + λ(u32Modulus)*x = 1
    //  d*e = x*λ(u32Modulus) + 1 (x is a mult number) => (d*e) % λ(u32Modulus) = 1
    */
    u32DecryptionKey = 1;
    while((uint64_t)((uint64_t)u32DecryptionKey*(uint64_t)u16EncryptionKey)%(uint64_t)u32Modulus_CarmichaelFunction != 1)
    {
        u32DecryptionKey++;
    }
    #if(DEBUG>1)
    printf("u32DecryptionKey: %d\n", u32DecryptionKey);
    #endif
    // copy results
    xRsaModule.u32Modulus = u32Modulus;
    xRsaModule.u32EncryptionKey = u16EncryptionKey;
    xRsaModule.u32DecryptionKey = u32DecryptionKey;

    return xRsaModule;
}

xEncryptionRsaModule_t xSetCustomRsaModule(uint32_t u32Modulus, uint32_t u32EncryptKey, uint32_t u32DecrypKey)
{
    xEncryptionRsaModule_t xRsaModule;
    xRsaModule.u32Modulus = u32Modulus;
    xRsaModule.u32EncryptionKey = u32EncryptKey;
    xRsaModule.u32DecryptionKey = u32DecrypKey;
    return xRsaModule;
}

void vEncryption(xEncryptionRsaModule_t xRsaModule, uint8_t *pu8Buffer, uint32_t u32BufferLen, uint64_t *p64EncryptedBuffer)
{
    uint32_t i, j;
    for(i=0; i<u32BufferLen; i++)
    {
        //method 1
        /*
        // this method won't work since the values are so huge but this is the implimentation of the encryption function 
        //   c = m^e mod(n),
        //      c: encrypted message
        //      m: message
        //      e: encryption key
        //      n: modulus
        */
        //p64EncryptedBuffer[i] = pow((uint64_t)pu8Buffer[i], (uint64_t)xRsaModule.u32EncryptionKey);
        //printf("pow of '%c'= %u\n", pu8Buffer[i], p64EncryptedBuffer[i]);
        //p64EncryptedBuffer[i] = p64EncryptedBuffer[i] % (uint64_t)xRsaModule.u32Modulus;
            
        // method 2 (fast and working method with huge number)
        p64EncryptedBuffer[i] = 1;
        for(j=0; j<xRsaModule.u32EncryptionKey; j++)
        {
            p64EncryptedBuffer[i] = (p64EncryptedBuffer[i]*pu8Buffer[i]) % (uint64_t)xRsaModule.u32Modulus;
        }
    }
}

void vDecryption(xEncryptionRsaModule_t xRsaModule, uint64_t *pu64Buffer, uint32_t u32BufferLen, uint8_t *p8DecryptedBuffer)
{
    uint32_t i, j;
    uint64_t u64DecryptionHolder;

    for(i=0; i<u32BufferLen; i++)
    {
        // decription => m = c^d mod(n)
        //      m : message to send
        //      c : cipher text or crypted message
        //      d : decryption key
        //      n : modulus
        u64DecryptionHolder = 1;
        for(j=0; j<xRsaModule.u32DecryptionKey; j++)
        {
            u64DecryptionHolder = (u64DecryptionHolder*pu64Buffer[i]) % (uint64_t)xRsaModule.u32Modulus;
        }
        p8DecryptedBuffer[i] = (uint8_t)u64DecryptionHolder;
    }
}
