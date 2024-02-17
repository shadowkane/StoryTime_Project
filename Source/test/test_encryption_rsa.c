#include "encryption_RSA.h"
#include <assert.h>
#include <stdio.h>

#define VALIDE_MODULUS 60138581
#define VALIDE_ENCRYPUION 54719
#define VALIDE_DECRYPTION 6175583

#define MESSAGE_LEN 11
uint8_t pu8ValidMessage[MESSAGE_LEN] = {'D','E','A','D','b','e','e','f','0','-','9'};
uint64_t pu64ValidCipher[MESSAGE_LEN] = {34442301,45623862,59782298,34442301,51338302,59664878,59664878,32089094,58812583,12376631,33228134};

int test_encryption_rsa();

int test_encryption_rsa()
{
    xEncryptionRsaModule_t xRsaModule = {0, 0, 0};
    uint8_t pu8TestMessage[MESSAGE_LEN];
    uint64_t pu64TestCipher[MESSAGE_LEN];

    //printf("------------- Start encryption_RSA Test -------------\n");

    // Test xRsaKeyGenerator(...)
    xRsaModule = xRsaKeyGenerator(0x100, 0x200, 73);
    // RsaModule's variables should be changed to value different than 0
    assert(xRsaModule.u32Modulus!=0);
    assert(xRsaModule.u32EncryptionKey!=0);
    assert(xRsaModule.u32DecryptionKey!=0);
    printf("test_encryption_RSA, test function: xRsaKeyGenerator(...) ====> PASS\n");

    // Test xSetCustomRsaModule(...)
    xRsaModule = xSetCustomRsaModule(VALIDE_MODULUS, VALIDE_ENCRYPUION, VALIDE_DECRYPTION);
    // RsaModule's variables should be changed to what we set
    assert(xRsaModule.u32Modulus==VALIDE_MODULUS);
    assert(xRsaModule.u32EncryptionKey==VALIDE_ENCRYPUION);
    assert(xRsaModule.u32DecryptionKey==VALIDE_DECRYPTION);
    printf("test_encryption_RSA, test function: xSetCustomRsaModule(...) ====> PASS\n");

    // Test vEncryption(...)
    vEncryption(xRsaModule, pu8ValidMessage, MESSAGE_LEN, pu64TestCipher);
    for(int i=0; i<MESSAGE_LEN; i++)
    {
        assert(pu64TestCipher[i]==pu64ValidCipher[i]);
    }
    printf("test_encryption_RSA, test function: vEncryption(...) ====> PASS\n");

    // Test vDecryption(...)
    vDecryption(xRsaModule, pu64ValidCipher, MESSAGE_LEN, pu8TestMessage);
    for(int i=0; i<MESSAGE_LEN; i++)
    {
        assert(pu8TestMessage[i]==pu8ValidMessage[i]);
    }
    printf("test_encryption_RSA, test function: vDecryption(...) ====> PASS\n");

    //printf("------------- END encryption_RSA Test -------------\n");

    return 0;
}