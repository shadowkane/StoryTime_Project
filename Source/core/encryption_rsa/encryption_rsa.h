#ifndef CORE_ENCRYPTION_RSA_ENCRYPTION_RSA_H_
#define CORE_ENCRYPTION_RSA_ENCRYPTION_RSA_H_

#include <stdio.h>
#include <stdint.h>

typedef struct _xRsaModule_t
{
    uint32_t u32Modulus;
    uint32_t u32EncryptionKey;
    uint32_t u32DecryptionKey;
} xEncryptionRsaModule_t;

/*!
    @function   vRsaKeyGenerator
    @brief      Generate Modulus and the public and privet keys
    @param      u16MinValue  lowest possible value you going to encrypte (if it's a char, then it's the lowest ASCII code)
    @param      u16MaxValue  greatest possible value you going to encrypte (if it's a char, then it's the greatest ASCII code)
    @param      u16RandomValue  just a random value to use for the key generation
    @result     return an RSA module with a generated keys (modulus, encryption, decryption)
    @attention  make sure u32MaxValue > u32MinValue
*/
xEncryptionRsaModule_t xRsaKeyGenerator(uint16_t u16MinValue, uint16_t u16MaxValue, uint16_t u16RandomValue);

/*!
    @function   vSetCustomRsaModule
    @brief      Set custom RAS key pair
    @param      u32Modulus
    @param      u32EncryptKey
    @param      u32DecrypKey
    @result     return an RSA module with the passed parameters
*/
xEncryptionRsaModule_t xSetCustomRsaModule(uint32_t u32Modulus, uint32_t u32EncryptKey, uint32_t u32DecrypKey);


/*!
    @function   vEncryption
    @brief      Encryption function
    @param      xRsaModule  RSA module that alraedy have encryption key (generated or customly set by user)
    @param      pu8Buffer Buffer (message) you want to encrypt
    @param      u32BufferLen Buffer length
    @param      p64EncryptBuffer Encryption result will be stored in this buffer (Cipher message)
*/
void vEncryption(xEncryptionRsaModule_t xRsaModule, uint8_t *pu8Buffer, uint32_t u32BufferLen, uint64_t *p64EncryptedBuffer);

/*!
    @function   vDecryption
    @brief      Decryption function
    @param      xRsaModule  RSA module that alraedy have encryption key (generated or customly set by user)
    @param      pu64Buffer Encrypted buffer (Cipher message) you want to decrypt
    @param      u32BufferLen Buffer length
    @param      p8DecryptedBuffer Decryption result will be stored in this buffer (read message)
*/
void vDecryption(xEncryptionRsaModule_t xRsaModule, uint64_t *pu64Buffer, uint32_t u32BufferLen, uint8_t *p8DecryptedBuffer);

#endif // CORE_ENCRYPTION_RSA_ENCRYPTION_RSA_H_