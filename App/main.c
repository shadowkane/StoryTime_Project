/*
    Auther: Habib
    Date: 12/11/2023

    Project description:    A simple application to encrypt/decrypt file and after the decryption, the file will be displayed in terminal.
    Project idea:   The idea behind this project is to provide a unique and interresting way to send an application job to your favorite company that you would like to apply to.
                    The program will read a file, so you need to write a file talking about yourself, your ambisions and enthusiasm in a short and not boring way,
                    and since the aboutMe file need to be send and we need to make sure the reader doesn't just open this file and read it in a monotonous way where all the magic will disapear.
                    we need to encrypte this file. so, yeap, i have wrote the RSA cryptosystem to protect the aboutyou sharm. Also i had to think about the reader since he's the RH manager and no way he has the time to deal with programing complexicities,
                    also no one will just open an executable file from a random person. for those reasons, the project need to be portable, easy to build and reliable.
                    So, this is my idea of an interesting and a unique job application.
                    Note: don't forget your CV ;).
    How to:
        + first of all, create file "aboutMe.txt" and write about yourself, You can change the file name in the config section.
        + Second step: since you as a job applicant need to use this app first to generate the public and privet key and to encrypte your file, you need to define the symbole "BUILD_FOR_APPLICANT"
            to make it eazy, it's already defined in config section, you only need to uncomment it. This phase called "encryption mode" or "applicant mode"
            run your application in encryption mode you will get:
                - encryption/decryption parameters:
                    public key [modulus(or n)= <number> | encryption key(or d)=<number>]
                    prived key [modulus(or n)= <number> | decryption key(or d)=<number>] 
                - aboutMe_encrypted.txt: this is the aboutMe file after encrytion and it should be send along side with the application
        + Third step: this run intended for the reader, so it's just building the app and run the executable file
                - you as a job applicant, you need to provide the public key so the reader doesn't need to write any arguement or edit anything in this code. (edit config section)
                - the applicant can build this App for the reader, just remeber to set the correct parameters and config and you can test it.

*/


/*
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


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

/* ----- Config ----- */
#define ABOUT_ME_FILE_NAME "aboutMe.txt"
#define ABOUT_ME_ENCRYPTED_FILE_NAME "aboutMe_encrypted.txt"
#define BUILD_FOR_APPLICANT // comment this line when you want to build for reader and before you send this project
//#define ENABLE_TEST // use this macro to include your tests and comment to easy exclude them from build
#define DEBUG_LEVEL 1 // debug levels >= 0, 0 mean no debug message will be shown
#if !defined(BUILD_FOR_APPLICANT)
// provide the correct privet key for the decryption
#define PRIVET_KEY_DECRYPTION_KEY   0  
#define PRIVET_KEY_MODULUS          0
    // #if((PRIVET_KEY_DECRYPTION_KEY<0)||(PRIVET_KEY_MODULUS<0))
    //     #error Public key required, you as a job applicant, you need to provide it. if you are the RH manager, please edit line 52-53 with correct privet key values
    // #endif
#endif

/* ----- Define ----- */
#ifdef _WIN32
#define delay_ms(s)   Sleep(s)
#else
#define delay_ms(s)   usleep(s*1000)
#endif
/*   RSA   */
#if defined(BUILD_FOR_APPLICANT)
// 1st prime number initialization
#define UPPER_RANDOM_VALUE  0x200
#define LOWER_RANDOM_VALUE  0x100  // since message are ASCII which mean it's between 0x00 and 0xFF so the lowest value shoudl be more 0xFF
// 2nd prime number initialization
#define PRIME1_PRIME2_DIFFERENCE    73    // random gap (or difference) between prime number 1 and prime number 2
#endif 
/*   Narration Tags or diractives   */
#define NARRATION_TAG_HINT              '$'
#define NARRATION_TAG_ENABLE_RYTHM      "start_rythm"
#define NARRATION_TAG_DISABLE_RYTHM     "stop_rythm"
#define NARRATION_TAG_PAUSE_S           "pause_s=" 
#define NARRATION_TAG_PAUSE_MS          "pause_ms="
#define NARRATION_TAG_SKIP_REQUEST      "skip_request="
#define NARRATION_TAG_SKIP_END          "skip_end"
//#define NARRATION_TAG_YES_NO_QUESTION   "yes_no_question="  

/* ----- Function Declaration ----- */
/*   RSA   */
#if defined(BUILD_FOR_APPLICANT)
// check in value is a prime number
bool bIsPrime(int iVal);
//bool bIsPrime_fast(int n); // fast prime number check algorithm (src: https://en.wikipedia.org/wiki/Primality_test)
// GCD (greatest common divisor) function
unsigned int uGCD(int a, int b);
//int iGCD(int a, int b); // other method to find GCD
// LCM (Least Common Multiple)
int iLCM(int a, int b);
// Generate the public and privet keys
void vRsaKeyGenerator(uint32_t* u32Modulus_out, uint16_t* u16EncryptionKey_out, uint32_t* u32DecryptionKey_out);
// encryption function
void vEncryption(uint32_t u32Modulus, uint32_t u32EncryptKey, uint8_t* pu8RawBuffer, uint32_t u32BufferLen, uint64_t* p64EncryptBuffer);
#endif
// Decryption function
void vDecryption(uint32_t u32Modulus, uint32_t u32DecrypKey, uint64_t* p64EncryptBuffer, uint32_t u32BufferLen, uint8_t* p64OutputBuffer);
/*   Tools   */
bool bIsPositiveNumber(char* pcNumberInStr, int iSize);
uint32_t u32InputPositiveValue();
bool bYesNoQuestion(const char* questionMsg);
void vNarration(char cChar);
void vExitProgram(int iErrorValue);

/* ----- Variable ----- */
/*   RSA   */
// privet key parts
uint32_t u32DecryptionKey;
// public key parts
#if defined(BUILD_FOR_APPLICANT)
uint16_t u16EncryptionKey;
#endif
uint32_t u32Modulus;
/*   File   */
#if defined(BUILD_FOR_APPLICANT)
FILE *pRawFile;
#endif
FILE *pEncryptedFile;
/* */
char cCharHolder;
uint64_t p64CipherHolder;
int iFunctionResult;
int iReadcounter;


void main(int argc, char *argv[]){
    
    //Initialization
    system("MODE 100,40");
    SMALL_RECT xTerminalSize = {0, 0, 100, 40};   //New dimensions for window in 8x12 pixel chars (Left, Top, Right, Bottom)
    SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, &xTerminalSize);   //Set new size for window
    delay_ms(1000);

    #if defined(BUILD_FOR_APPLICANT)
    printf("Getting Start...\n");

    /***********    Applicant section    ***********/
    // generate privet and public keys
    vRsaKeyGenerator(&u32Modulus, &u16EncryptionKey, &u32DecryptionKey);
    /* Print results */
    // Prive key partes
    printf("Prive key: modulus(or n)= %d | decryption key(or d)= %d\n", u32Modulus, u32DecryptionKey);
    // public key partes
    printf("Public key: modulus(or n)= %d | encryption key(or e)= %d\n", u32Modulus, u16EncryptionKey);

    #if defined(ENABLE_TEST)
    /* ---- test ---- */
    /* String */
    #define iMsgLen 10
    char pcMsg[iMsgLen] = {'H', 'e', 'l', 'l', 'o', ' ', 'M', 'S', 'G', '0'};
    uint64_t p64CipherMsg[iMsgLen];
    char pcMsgAfterDecryption[iMsgLen];
    int i;
    /* Encryption */
    vEncryption(u32Modulus, u16EncryptionKey, &(pcMsg[0]), iMsgLen, &(p64CipherMsg[0]));
    for(i=0; i<iMsgLen; i++)
    {
        printf("Cipher text of '%c'= %d\n", pcMsg[i], p64CipherMsg[i]);
    }

    /* Decryption */
    vDecryption(u32Modulus, u32DecryptionKey, &(p64CipherMsg[0]), iMsgLen, &(pcMsgAfterDecryption[0]));
    for(i=0; i<iMsgLen; i++)
    {
        printf("text of '%d= %c\n", p64CipherMsg[i], pcMsgAfterDecryption[i]);
    }
    #endif

    /* ---- File Encryption for applicant ---- */
    // Original file
    pRawFile = fopen(ABOUT_ME_FILE_NAME, "r");
    if(pRawFile==NULL)
    {
        printf("Couldn't open %s file\n", ABOUT_ME_FILE_NAME);
        vExitProgram(-1);
    }
    fseek(pRawFile, 0, SEEK_END);
    if(ftell(pRawFile)==0)
    {
        printf("ERROR: File %s is empty\n", ABOUT_ME_FILE_NAME);
        fclose(pRawFile);
        vExitProgram(-1);
    }
    else
    {
        fseek(pRawFile, 0, SEEK_SET); // set back file pointer position at the start
    }
    // Encrypted file
    pEncryptedFile = fopen(ABOUT_ME_ENCRYPTED_FILE_NAME, "w+");
    if(pEncryptedFile==NULL)
    {
        printf("Couldn't create %s file\n", ABOUT_ME_ENCRYPTED_FILE_NAME);
        vExitProgram(-1);
    }
    else
    {
        printf("New file %s created\n", ABOUT_ME_ENCRYPTED_FILE_NAME);
    }

    // File encryption
    printf("Start file encryption...\n");
    while(1){
        iFunctionResult = fgetc(pRawFile);
        if(iFunctionResult==EOF){
            break;
        }
        cCharHolder = (char)iFunctionResult;
        vEncryption(u32Modulus, u16EncryptionKey, &cCharHolder, 1, &p64CipherHolder);
        #if(DEBUG_LEVEL>2)
        printf("Cipher text of '%c'= %d\n", cCharHolder, p64CipherHolder);
        #endif
        fprintf(pEncryptedFile, "%d,", p64CipherHolder);
    }
    printf("Finish.\n");
    fclose(pRawFile);
    fclose(pEncryptedFile);
    ///ask for permission to run preview
    if(bYesNoQuestion("Do you want to see a preview?"))
    {
        printf("----------------------------------- Decryption preview -----------------------------------\n");
    #endif

     /***********    Applicant and Reader section    ***********/
    /* ---- File Decryption ---- */
    #if !defined(BUILD_FOR_APPLICANT)
    if(argc==3)
    {
        // Decryption key
        if(!bIsPositiveNumber(argv[1], strlen(argv[1])))
        {
            printf("Incorrect arguement %s\n", argv[1]);
            vExitProgram(-1);
        }
        else
        {
            sscanf(argv[1], "%d", &u32DecryptionKey);
        }
        // Modulus
         if(!bIsPositiveNumber(argv[2], strlen(argv[2])))
        {
            printf("Incorrect arguement %s\n", argv[2]);
            vExitProgram(-1);
        }
        else
        {
            sscanf(argv[2], "%d", &u32Modulus);
        }
    }
    // check if PRIVET_KEY_DECRYPTION_KEY and PRIVET_KEY_MODULUS are defined, if not, that mean we don't have saved keys
    #if (defined(PRIVET_KEY_DECRYPTION_KEY) && defined(PRIVET_KEY_MODULUS))
        // check if both keys are defined with positive values (if they just defined without any value, the project will not even compile)
        else if((PRIVET_KEY_DECRYPTION_KEY>0) && (PRIVET_KEY_MODULUS>0))
        {
            u32DecryptionKey = PRIVET_KEY_DECRYPTION_KEY;
            u32Modulus = PRIVET_KEY_MODULUS;
        }
    #endif
    else
    {
 
        printf("Enter the Decryption key number=");
        u32DecryptionKey = u32InputPositiveValue();
        printf("Enter the Modulus number=");
        u32Modulus = u32InputPositiveValue();

    }
        #if(DEBUG_LEVEL>1)
    printf("Decryption key=%d , Modulus=%d\n", u32DecryptionKey, u32Modulus);
        #endif
    #endif
  

    // open file
    pEncryptedFile = fopen(ABOUT_ME_ENCRYPTED_FILE_NAME, "r");
    if(pEncryptedFile==NULL)
    {
        printf("Couldn't open %s file\n", ABOUT_ME_ENCRYPTED_FILE_NAME);
        vExitProgram(-1);
    }
    fseek(pEncryptedFile, 0, SEEK_END);
    if(ftell(pEncryptedFile)==0)
    {
        printf("ERROR: File %s is empty\n", ABOUT_ME_ENCRYPTED_FILE_NAME);
        fclose(pEncryptedFile);
        vExitProgram(-1);
    }
    else
    {
        fseek(pEncryptedFile, 0, SEEK_SET); // set back file pointer position at the start
    }
    // read, encrypte and display file
    while(1)
    {
        if(fscanf(pEncryptedFile,"%d", &p64CipherHolder)<1)
        {
            break;
        }
        fgetc(pEncryptedFile);// use it to move to the next char. (to skip the separator)
        vDecryption(u32Modulus, u32DecryptionKey, &p64CipherHolder, 1, &cCharHolder);
        vNarration(cCharHolder);
    }
    
    #if defined(BUILD_FOR_APPLICANT)
        printf("\n----------------------------------- End of preview -----------------------------------\n");
    }//permission question end
    #endif

    // Exit
    vExitProgram(0);
}

#if defined(BUILD_FOR_APPLICANT)
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

/*
bool bIsPrime_fast(int n)
{
    if (n == 2 || n == 3)
        return true;

    if (n <= 1 || n % 2 == 0 || n % 3 == 0)
        return false;

    for (int i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }

    return true;
}
*/
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
/*
int iGCD(int a, int b)
{
    printf("iGCD\n");
    // a and b need to be positive
    a = (a>0)? a:-a;
    b = (b>0)? b:-b;
    // keep substruct the small number from the big number until both are equal, the result is the GCD(a, b)
    while(a!=b)
    {
        if(a>b)
        {
            a-=b;
        }
        else
        {
            b-=a;
        }
    }

    return a; // a and b are equal, return one of them
}
*/

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

void vRsaKeyGenerator(uint32_t* u32Modulus_out, uint16_t* u16EncryptionKey_out,  uint32_t* u32DecryptionKey_out)
{
    // privet key parts
    uint16_t u16Prime1;
    uint16_t u16Prime2;
    uint32_t u32Modulus_CarmichaelFunction;
    uint32_t u32DecryptionKey;
    // public key parts
    uint32_t u32Modulus_CarmichaelFunction_CoprimeValue;
    uint16_t u16EncryptionKey;
    uint32_t u32Modulus;

 /* -step 1- generate 2 prime numbers */
    // set random generator seed
    srand(time(NULL));
    // get random value between UPPER_RANDOM_VALUE-LOWER_RANDOM_VALUE
    
    u16Prime1 = (rand() % (UPPER_RANDOM_VALUE - LOWER_RANDOM_VALUE + 1)) + LOWER_RANDOM_VALUE;
    do
    {
        u16Prime1++;
    } while (!bIsPrime(u16Prime1));
    #if(DEBUG_LEVEL>2)
    printf("Random value 1: %d\n", u16Prime1);
    #endif
    u16Prime2 = u16Prime1 + PRIME1_PRIME2_DIFFERENCE;
    do
    {
        u16Prime2++;
    } while (!bIsPrime(u16Prime2));
    #if(DEBUG_LEVEL>2)
    printf("Random value 2: %d\n", u16Prime2);
    #endif
   /*
    u16Prime2 = (rand() % (UPPER_RANDOM_VALUE - LOWER_RANDOM_VALUE + 1)) + LOWER_RANDOM_VALUE;
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
    #if(DEBUG_LEVEL>1)
    printf("Modulus : %d\n", u32Modulus);
    #endif
    /* -step 3- compute the Carmichael function λ(u32Modulus)*/
    // Since u32Modulus = u16Prime1 * u16Prime2 => λ(u32Modulus) = lcm(λ(u16Prime1), λ(u16Prime2))
    // and since u16Prime1 and u16Prime1 are prime, λ(u16Prime1) = u16Prime1 − 1, and λ(u16Prime2) = u16Prime2 − 1
    // so λ(u32Modulus) = lcm(u16Prime1 − 1, u16Prime2 − 1)
    u32Modulus_CarmichaelFunction = iLCM(u16Prime1-1, u16Prime2-1);
    #if(DEBUG_LEVEL>2)
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
    #if(DEBUG_LEVEL>2)
    printf("initial i16EncryptionKe: %d\n", u16EncryptionKey);
    #endif
    // find e, where e and λ(u32Modulus) are coprime using GCD(e, λ(u32Modulus)) = 1 
    while((uGCD((int)u16EncryptionKey, u32Modulus_CarmichaelFunction)!=1) && (u16EncryptionKey>2))
    {
        u16EncryptionKey++;
    }
    if(u16EncryptionKey<=2)
    {
        #if(DEBUG_LEVEL>0)
        printf("Failed to get the encryption key\n");
        #endif
        return; // -TODO- avoid hamburger functions style
    }
    #if(DEBUG_LEVEL>1)
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
    #if(DEBUG_LEVEL>1)
    printf("u32DecryptionKey: %d\n", u32DecryptionKey);
    #endif
    // copy results
    *u32Modulus_out = u32Modulus;
    *u16EncryptionKey_out = u16EncryptionKey;
    *u32DecryptionKey_out = u32DecryptionKey;
}

void vEncryption(uint32_t u32Modulus, uint32_t u32EncryptKey, uint8_t* pu8RawBuffer, uint32_t u32BufferLen, uint64_t* p64EncryptBuffer)
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
        //p64EncryptBuffer[i] = pow((uint64_t)pu8RawBuffer[i], (uint64_t)u32EncryptKey);
        //printf("pow of '%c'= %u\n", pu8RawBuffer[i], p64EncryptBuffer[i]);
        //p64EncryptBuffer[i] = p64EncryptBuffer[i] % (uint64_t)u32Modulus;
            
        // method 2 (fast and working method with huge number)
        p64EncryptBuffer[i] = 1;
        for(j=0; j<u32EncryptKey; j++)
        {
            p64EncryptBuffer[i] = (p64EncryptBuffer[i]*pu8RawBuffer[i]) % (uint64_t)u32Modulus;
        }
        
    }
}
#endif
void vDecryption(uint32_t u32Modulus, uint32_t u32DecrypKey, uint64_t* p64EncryptBuffer, uint32_t u32BufferLen, uint8_t* p64OutputBuffer)
{
    uint32_t i, j;
    uint64_t u64DecryptionHolder;
    for(i=0; i<u32BufferLen; i++)
    {
        u64DecryptionHolder = 1;
        for(j=0; j<u32DecrypKey; j++)
        {
            u64DecryptionHolder = (u64DecryptionHolder*p64EncryptBuffer[i]) % (uint64_t)u32Modulus;
        }
        p64OutputBuffer[i] = (uint8_t)u64DecryptionHolder;
    }
}

bool bIsPositiveNumber(char* pcNumberInStr, int iSize)
{
    bool bRetValue=true;
    int iCounter;
    // check if all characters are digits, and no negative number is allowed
    for(iCounter=0; iCounter<iSize; iCounter++)
    {
        if(pcNumberInStr[iCounter]<'0' || pcNumberInStr[iCounter]>'9')
        {
            bRetValue= false;
            break;
        }
    }
    return bRetValue;
}

uint32_t u32InputPositiveValue()
{
    uint32_t u32RetValue;
    char pcInputHolder[20];
    int iCounter;
    bool bIsValidInput;

    while(fgets(pcInputHolder, sizeof(pcInputHolder), stdin))
    {
        bIsValidInput=true;
        iCounter=0;
        // check if all characters are digits, and no negative number is allowed
        while(pcInputHolder[iCounter]!='\n')
        {
            if(pcInputHolder[iCounter]<'0' || pcInputHolder[iCounter]>'9')
            {
                bIsValidInput= false;
                break;
            }
            iCounter++;
        }
        if(bIsValidInput)
        {
            // this is a correct input, save it as integer and break from the loop
            if(sscanf(pcInputHolder, "%d", &u32RetValue)==1)
            {
                break;
            }
        }
        // if reach this position, mean we didn't break from the loop or/and the input is invalide
        printf("\nIncorrect input, try again. => ");
    }

    return u32RetValue;
}

bool bYesNoQuestion(const char* questionMsg)
{
    char cUserInput;
    printf("%s (y/n):", questionMsg); 
    while(1)
    {
        //scanf("%c", &cUserInput);
        cUserInput = getchar();
        fflush(stdin);
        if((cUserInput=='y')||(cUserInput=='Y'))
        {
            return true;
        }
        else if((cUserInput=='n')||(cUserInput=='N'))
        {
            return false;
        }
        else
        {
            printf("Incorrect answer, please enter y for yes or n for no=>");
        }
    }
}

// typedef struct _xNarrationOptions_t{
//     bool bIsRythmDisabled;

// }xNarrationOptions_t;

// bool bNarrationCommandHandler(char* pcCmd, xNarrationOptions_t* xNarrationOptions)
// {
//     bool bIsValidCmd = true;



//     return bIsValidCmd;
// }


void vNarration(char cChar)
{
    static char pcNarrationTagHolder[50];
    static int iNarrationTagHolderIterator=0;
    static bool bIsStoringNarrationOption = false;
    static bool bIsRythmDisabled = false;
    static bool bIsSkip = false;
    int iDelayDuration;
    
    /*
    Narration has 2 sections:
        + Configuration section: set uset configuration
        + Run section: just narrate the text
            - if display is 
        
    
    */

    /* Narration Config Section*/
    // start and end narration tag hint
    if(cChar==NARRATION_TAG_HINT)
    {
        // update storing narration option flag. when we detect tag hint it means it's the start or the end of the narration option
        bIsStoringNarrationOption = !bIsStoringNarrationOption;
        if(bIsStoringNarrationOption)
        {
            // in case it's the start of storing narration option command (or narration tag), we need to empty the buffer
            iNarrationTagHolderIterator = 0;
            pcNarrationTagHolder[0]='\0';
        }
        else
        {
            // in case it's the end of narration tag (end tag hint), we run the command
            // make sure to set the end of command
            pcNarrationTagHolder[iNarrationTagHolderIterator++]='\0';
            // run narration option handler
            //printf("cmd = %s\n", pcNarrationTagHolder);
            if(strcmp(pcNarrationTagHolder, NARRATION_TAG_ENABLE_RYTHM)==0 && !bIsSkip)
            {
                //printf("\nhere is enable rythm\n");
                bIsRythmDisabled = false;
            }
            else if(strcmp(pcNarrationTagHolder, NARRATION_TAG_DISABLE_RYTHM)==0 && !bIsSkip)
            {
                //printf("\nhere is disable rythm\n");
                bIsRythmDisabled = true;
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_PAUSE_S, strlen(NARRATION_TAG_PAUSE_S))==0 && !bIsSkip)
            {
                //printf("\nhere is pause s\n");
                // check if the given number is positive
                if(bIsPositiveNumber(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_S)))
                {
                    sscanf(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), "%d", &iDelayDuration);
                    printf("\ncmd=%s, pause duration str=%s, size=%d | pause duration=%d\n", pcNarrationTagHolder, pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_S), iDelayDuration);
                    delay_ms(iDelayDuration*1000);
                }
                #if defined(BUILD_FOR_APPLICANT)
                else
                {
                    printf("\n--You have a mistake here: pause duration should be a positive number!--\n");
                }
                #endif
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_PAUSE_MS, strlen(NARRATION_TAG_PAUSE_MS))==0 && !bIsSkip)
            {
                //printf("\nhere is pause ms\n");
                // check if the given number is positive
                if(bIsPositiveNumber(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_MS), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_MS)))
                {
                    sscanf(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_MS), "%d", &iDelayDuration);
                    //printf("\ncmd=%s, pause duration str=%s, size=%d | pause duration=%d\n", pcNarrationTagHolder, pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_MS), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_MS), iDelayDuration);
                    delay_ms(iDelayDuration);
                }
                #if defined(BUILD_FOR_APPLICANT)
                else
                {
                    printf("\n--You have a mistake here: pause duration should be a positive number!--\n");
                }
                #endif
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_SKIP_REQUEST, strlen(NARRATION_TAG_SKIP_REQUEST))==0 && !bIsSkip)
            {
                //printf("\nask skip\n");
                if(bYesNoQuestion(pcNarrationTagHolder+strlen(NARRATION_TAG_SKIP_REQUEST)))
                {
                    //printf("\nskip yes\n");
                    bIsSkip = true;
                }
                else
                {
                    //printf("\nskip no\n");
                    bIsSkip = false;
                }
            }
            else if(strcmp(pcNarrationTagHolder, NARRATION_TAG_SKIP_END)==0)
            {
                //printf("\nend skip\n");
                bIsSkip = false;
            }
            // else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_YES_NO_QUESTION, strlen(NARRATION_TAG_YES_NO_QUESTION))==0)
            // {
            //     if(bYesNoQuestion(pcNarrationTagHolder+strlen(NARRATION_TAG_YES_NO_QUESTION)))
            //     {

            //     }
            //     else
            //     {

            //     }
            // }
            #if defined(BUILD_FOR_APPLICANT)
            else
            {
                printf("\n--You have a mistake here: unknown narration command!--\n");
            }
            #endif
        }
    }
    else if(bIsStoringNarrationOption)
    {
        // store narration option
        pcNarrationTagHolder[iNarrationTagHolderIterator++]=cChar;
    }
    else if(!bIsSkip)
    {
        

        // each case should set those values:
        //  bIsDisplay: true to display char, false to not
        //  iDelayDuration: pause duration, it doesn't depend on bIsDisplay
        //  bIsQuestion: if true, the app will display a yes/no question and wait for the user answer
        switch (cChar)
        {
            case '\r':
            case '\n':
                iDelayDuration = 200;
                break;
            // case NARRATION_TAG_HINT:
            //     // don't display tag hint and no need to pause
            //     iDelayDuration = 0;
            //     // update storing narration option flag. when we detect tag hint it means it's the start or the end of the narration option
            //     bIsStoringNarrationOption = !bIsStoringNarrationOption;
            //     // we have nothing to do in case it's thart start of storing narration option command (or narration tag)
            //     // so in case it's the end of narration tag (end tag hint), we run the command
            //     if(!bIsStoringNarrationOption)
            //     {
            //         // run narration option handler

            //     }
                
            //     bIsQuestion = false; 
            //     break;
            default:
                iDelayDuration = 50;
                break;
        }

        if(!bIsRythmDisabled)
        {
            delay_ms(iDelayDuration);
        }
        printf("%c", cChar);
        fflush(stdout);
    }   
}

void vExitProgram(int iErrorValue)
{
    printf("Press Enter key to close the Application!");
    getchar();
    exit(iErrorValue);
}