/*
    Author: Habib
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
        + Second step: since you as a job applicant need to use this app first to generate the public and privet key and to encrypte your file, you need to define the symbole "BUILD_FOR_WRITER"
            to make it easy, it's already defined in config section, you only need to uncomment it. This phase called "encryption mode" or "applicant mode"
            run your application in encryption mode you will get:
                - encryption/decryption parameters:
                    public key [modulus(or n)= <number> | encryption key(or d)=<number>]
                    prived key [modulus(or n)= <number> | decryption key(or d)=<number>] 
                - aboutMe_encrypted.txt: this is the aboutMe file after encrytion and it should be send along side with the application
        + Third step: this run intended for the reader, so it's just building the app and run the executable file
                - you as a job applicant, you need to provide the public key so the reader doesn't need to write any arguement or edit anything in this code. (edit config section)
                - the applicant can build this App for the reader, just remeber to set the correct parameters and config and you can test it.

*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include "config.h"
#include "encryption_rsa.h"
#ifdef OS_WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
#endif

/* ----- Define ----- */
#ifdef OS_WINDOWS
#define delay_ms(ms)   Sleep(ms)
#else
#define delay_ms(ms)   usleep(ms*1000)
#endif
/*   RSA   */
#if defined(BUILD_FOR_WRITER)
#define RSA_MSG_MIN_VALUE   0       // min Value in ASCII
#define RSA_MSG_MAX_VALUE   0x00ff  // max Value in ASCII 255
#define RSA_RANDOM_VALUE    73      // just a random value requested by the encryption_rsa library
#endif 
/*   Narration Tags or diractives   */
#define NARRATION_TAG_HINT                  '$'
#define NARRATION_TAG_ENABLE_RYTHM          "start_rythm"
#define NARRATION_TAG_DISABLE_RYTHM         "stop_rythm"
#define NARRATION_TAG_RYTHM_DEFAULT         "rythm_default"
#define NARRATION_TAG_RYTHM_S               "rythm_s="
#define NARRATION_TAG_RYTHM_MS              "rythm_ms="
#define NARRATION_TAG_PAUSE_S               "pause_s=" 
#define NARRATION_TAG_PAUSE_MS              "pause_ms="
#define NARRATION_TAG_SKIP_REQUEST_YES      "skip_on_yes="
#define NARRATION_TAG_SKIP_REQUEST_NO       "skip_on_no="
#define NARRATION_TAG_SKIP_END              "skip_end"
#define NARRATION_TAG_TEXT_STYLE_DEFAULT    "text_style_default"
#define NARRATION_TAG_TEXT_COLOR            "text_color="
#define NARRATION_TAG_TEXT_COLOR_SIZE   11
#define NARRATION_TAG_TEXT_FONT             "text_font="
#define NARRATION_TAG_TEXT_FONT_SIZE    10
//#define NARRATION_TAG_YES_NO_QUESTION     "yes_no_question="  

#define NARRATION_RYTHM_DEFAULT_DURATION    50
/* ----- New type ----- */
// Colors
typedef enum _enTextColor_t
{
    c_black, c_white, c_red, c_green, c_blue, c_yellow, c_magenta, c_cyna,
    c_black_bright, c_white_bright, c_red_bright, c_green_bright, c_blue_bright, c_yellow_bright, c_magenta_bright, c_cyna_bright,
    c_default, c_maxColorNbr
}enTextColor_t;
// Color tag list
const char* TextColors_tags[c_maxColorNbr] = {"black", "white", "red", "green", "blue", "yellow", "magenta", "cyna",
                                            "bright_black", "bright_white", "bright_red", "bright_green", "bright_blue", "bright_yellow", "bright_magenta", "bright_cyna",
                                            "default"};
// color cmds
#ifdef OS_WINDOWS
const WORD TextColors_windows[c_maxColorNbr] = {0x0000, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED, FOREGROUND_RED, FOREGROUND_GREEN, FOREGROUND_BLUE, FOREGROUND_RED | FOREGROUND_GREEN, FOREGROUND_BLUE | FOREGROUND_RED, FOREGROUND_BLUE | FOREGROUND_GREEN,
                                                0x0000 | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_INTENSITY, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY};
#endif
const char* TextColors_ansi[c_maxColorNbr] = {"\x1B[30m", "\x1B[37m", "\x1B[31m", "\x1B[32m", "\x1B[34m", "\x1B[33m", "\x1B[35m", "\x1B[36m", 
                                            "\x1B[90m", "\x1B[97m", "\x1B[91m", "\x1B[92m", "\x1B[94m", "\x1B[93m", "\x1B[95m", "\x1B[96m"};
// Fonts
typedef enum _enTextFont_t
{
    f_bold, f_italic, f_underline, f_default, f_maxFontNbr
}enTextFont_t;
// font tag list
const char* TextFonts_tags[f_maxFontNbr] = {"bold", "italic", "underline", "default"};
// font cmds
const char* TextFonts_ansi[f_maxFontNbr] = {"\x1B[1m", "\x1B[3m", "\x1B[4m", "\033[22;24m"};
// default text config ANSI cmd
#define DEFAULT_TEXT    "\x1B[0m"

/* ----- Function Declaration ----- */
/*   Tools   */
bool bIsPositiveNumber(char* pcNumberInStr, int iSize);
uint32_t u32GetPositiveValue();
bool bGetChar(char* pcOut);
void vSetTextSyleToDefault();
void vSetTextColor(enTextColor_t color);
void vSetTextFont(enTextFont_t font);
bool bYesNoQuestion(const char* questionMsg);
void vNarration(char cChar);
void vExitProgram(int iErrorValue);

/* ----- Variable ----- */
/*   RSA   */
xEncryptionRsaModule_t xRsaModule;
/*   File   */
#if defined(BUILD_FOR_WRITER)
FILE *pRawFile;
#endif
FILE *pEncryptedFile;
/* */
char cCharHolder;
uint64_t u64CipherHolder;
int iFunctionResult;
int iReadcounter;
bool bIsUseAnsi;
int iRythmDuration;

int main(int argc, char *argv[]){
    int iIterator;
    uint32_t u32DecryptionKey;
    uint32_t u32Modulus;

    /* Initialization */
    bIsUseAnsi = true;
    iRythmDuration=NARRATION_RYTHM_DEFAULT_DURATION;   

    // Terminal Title
    #ifdef OS_WINDOWS
    SetConsoleTitle(CT_TITLE);
    #else
    printf("\x1b]0;" CT_TITLE "\007");
    #endif
    // Terminal configuration
    #ifdef OS_WINDOWS
    HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO xBufferInfo;
    DWORD dwMode = 0;
    if (hndl != INVALID_HANDLE_VALUE)
    {
        // set terminal dimension
        if(GetConsoleScreenBufferInfo(hndl,&xBufferInfo))
        {
            #if(DEBUG>3)
            printf("Console Buffer Width: %d\n", xBufferInfo.dwSize.X);
            printf("Console Buffer Height: %d\n", xBufferInfo.dwSize.Y);
            printf("Console window left : %d\n", xBufferInfo.srWindow.Left);
            printf("Console window top  : %d\n", xBufferInfo.srWindow.Top);
            printf("Console window right: %d\n", xBufferInfo.srWindow.Right);
            printf("Console window bot  : %d\n", xBufferInfo.srWindow.Bottom);
            #endif
            xBufferInfo.srWindow.Bottom = 55;
            SetConsoleWindowInfo(hndl,true, &xBufferInfo.srWindow);
        }
        // Set output mode to handle virtual terminal sequences
        if (!GetConsoleMode(hndl, &dwMode))
        {
            #if(DEBUG>1)
            printf("Error: Couldn't get console mode\n");
            #endif
        }
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hndl, dwMode))
        {
            #if(DEBUG>1)
            printf("Error: Couldn't set console mode to enable virtual terminal processing\n");
            #endif
            // since virtual terminal processing in not enabled in windows, we turn off the use of ANSI commands 
            bIsUseAnsi = false;
        }
    }
        #if(DEBUG>1)
    else
    {
        printf("Invalid STD out handle\n");
    }
        #endif
    #else
    // if this ANSI command didn't work, the terminal size won't change, this is not a big problem, so we can ignore it for now --TODO: minor--
    //printf("\e[3;200;120t"); 
    //printf("\e[4;200;120t");
    printf("\e[8;200;120t");
    #endif
    // reset all terminal text attributes
    vSetTextSyleToDefault();

    delay_ms(1000);

    /***********    Applicant section    ***********/
    #if defined(BUILD_FOR_WRITER)
    printf("Getting Start...\n");
    // generate privet and public keys
    xRsaModule = xRsaKeyGenerator(RSA_MSG_MIN_VALUE, RSA_MSG_MAX_VALUE, RSA_RANDOM_VALUE);
    /* Print results */
    // Decription key partes (Prive key partes)
    printf("Decryption key pair(save me):\n\
        decryption key= %d | modulus= %d\n", xRsaModule.u32DecryptionKey, xRsaModule.u32Modulus);
    // Encryption key partes (public key partes)
    printf("Encryption key pair:\n\
        encryption key=%d | modulus= %d\n", xRsaModule.u32EncryptionKey, xRsaModule.u32Modulus);
    printf("  >>>> IMPORTANT: Remember to save the Decryption key pair to use it for the reader version!! <<<<  \n");

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
    pRawFile = fopen(RAW_FILE_NAME, "r");
    if(pRawFile==NULL)
    {
        printf("Couldn't open %s file\n", RAW_FILE_NAME);
        vExitProgram(-1);
    }
    fseek(pRawFile, 0, SEEK_END);
    if(ftell(pRawFile)==0)
    {
        printf("ERROR: File %s is empty\n", RAW_FILE_NAME);
        fclose(pRawFile);
        vExitProgram(-1);
    }
    else
    {
        fseek(pRawFile, 0, SEEK_SET); // set back file pointer position at the start
    }
    // Encrypted file
    pEncryptedFile = fopen(ENCRYPTED_FILE_NAME, "w+");
    if(pEncryptedFile==NULL)
    {
        printf("Couldn't create %s file\n", ENCRYPTED_FILE_NAME);
        vExitProgram(-1);
    }
    else
    {
        printf("New file %s created\n", ENCRYPTED_FILE_NAME);
    }

    // File encryption
    printf("Start file encryption...\n");
    while(1){
        iFunctionResult = fgetc(pRawFile);
        if(iFunctionResult==EOF){
            break;
        }
        cCharHolder = (char)iFunctionResult;
        vEncryption(xRsaModule, (uint8_t*)&cCharHolder, 1, &u64CipherHolder);
        #if(DEBUG>2)
        printf("Cipher text of '%c'= %"PRIu64"\n", cCharHolder, u64CipherHolder);
        #endif
        fprintf(pEncryptedFile, "%"PRIu64",", u64CipherHolder);
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
    #if !defined(BUILD_FOR_WRITER)
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
        u32DecryptionKey = u32GetPositiveValue();
        printf("Enter the Modulus number=");
        u32Modulus = u32GetPositiveValue();

    }
    #if(DEBUG>1)
    printf("Decryption key=%d , Modulus=%d\n", u32DecryptionKey, u32Modulus);
    #endif
    // we don't need encryption key, so, we set it to -1
    xRsaModule = xSetCustomRsaModule(u32Modulus,-1, u32DecryptionKey);
    #endif // !defined(BUILD_FOR_WRITER)
  

    // open file
    pEncryptedFile = fopen(ENCRYPTED_FILE_NAME, "r");
    if(pEncryptedFile==NULL)
    {
        printf("Couldn't open %s file\n", ENCRYPTED_FILE_NAME);
        vExitProgram(-1);
    }
    fseek(pEncryptedFile, 0, SEEK_END);
    if(ftell(pEncryptedFile)==0)
    {
        printf("ERROR: File %s is empty\n", ENCRYPTED_FILE_NAME);
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
        if(fscanf(pEncryptedFile,"%"PRIu64, &u64CipherHolder)<1)
        {
            break;
        }
        fgetc(pEncryptedFile);// use it to move to the next char. (to skip the separator)
        vDecryption(xRsaModule, &u64CipherHolder, 1, (uint8_t*)&cCharHolder);
        vNarration(cCharHolder);
    }
    
    #if defined(BUILD_FOR_WRITER)
        printf("\n----------------------------------- End of preview -----------------------------------\n");
    }//permission question end
    #endif

    // Exit
    vExitProgram(0);
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

uint32_t u32GetPositiveValue()
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

// Get or read char from user input and clear stdin buffer for the next read
// if user entered only one char, the function will return true
// if user entered more than one char, it will keep the first char and return false
bool bGetChar(char* pcOut)
{
    char cChar;
    int icharCounter;

    // read first char
    *pcOut = getchar();
    icharCounter=1;
    // get stdout buffer size and clear it
    // since the user need to press enter to pass char, so at least we have one char ('\n') still in stdin buffer
    do{
        cChar = getchar();
        icharCounter++;
    }while(cChar!='\n' && cChar!=EOF);
    // return result
    if(icharCounter==2)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void vSetTextSyleToDefault()
{
    if(bIsUseAnsi)
    {
        printf(DEFAULT_TEXT); 
    }
    else
    {
        #ifdef OS_WINDOWS
        // since only color can change when using windows commands so the default is to change back the text color to white
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TextColors_windows[c_white]);
        #endif
    }
}

void vSetTextColor(enTextColor_t color)
{
    if(color<c_maxColorNbr)
    {
        if(color == c_default)
        {
            color = c_white;
        }
        if(bIsUseAnsi)
        {
            printf(TextColors_ansi[color]);
        }
        else
        {
            #ifdef OS_WINDOWS
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), TextColors_windows[color]);
            #endif
        }
    }    
}

void vSetTextFont(enTextFont_t font)
{
    if(bIsUseAnsi)
    {
        printf(TextFonts_ansi[font]);
    }
}

bool bYesNoQuestion(const char* questionMsg)
{
    char cUserInput;
    printf("%s (y/n):", questionMsg); 
    while(1)
    {
        //scanf("%c", &cUserInput);
        //cUserInput = getchar();
        if(bGetChar(&cUserInput))
        {
            if((cUserInput=='y')||(cUserInput=='Y'))
            {
                return true;
            }
            else if((cUserInput=='n')||(cUserInput=='N'))
            {
                return false;
            }
        }
        printf("Incorrect answer, please enter y for yes or n for no=>");
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
    int iIterator;

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
            else if(strcmp(pcNarrationTagHolder, NARRATION_TAG_RYTHM_DEFAULT)==0 && !bIsSkip)
            {
                //printf("\nhere is rythm default duration\n");
                iRythmDuration = NARRATION_RYTHM_DEFAULT_DURATION;
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_RYTHM_S, strlen(NARRATION_TAG_RYTHM_S))==0 && !bIsSkip)
            {
                //printf("\nhere is rythm s\n");
                // check if the given number is positive
                if(bIsPositiveNumber(pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_RYTHM_S)))
                {
                    sscanf(pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_S), "%d", &iRythmDuration);
                    //printf("\ncmd=%s, rythm duration str=%s, size=%d | rythm duration=%d\n", pcNarrationTagHolder, pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_RYTHM_S), iRythmDuration);
                    iRythmDuration = iRythmDuration*1000;
                }
                #if defined(BUILD_FOR_WRITER)
                else
                {
                    printf("\n--You have a mistake here: rythm duration should be a positive number!--\n");
                }
                #endif
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_RYTHM_MS, strlen(NARRATION_TAG_RYTHM_MS))==0 && !bIsSkip)
            {
                //printf("\nhere is rythm ms\n");
                // check if the given number is positive
                if(bIsPositiveNumber(pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_MS), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_RYTHM_MS)))
                {
                    sscanf(pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_MS), "%d", &iRythmDuration);
                    //printf("\ncmd=%s, rythm duration str=%s, size=%d | rythm duration=%d\n", pcNarrationTagHolder, pcNarrationTagHolder+strlen(NARRATION_TAG_RYTHM_MS), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_RYTHM_MS), iRythmDuration);
                }
                #if defined(BUILD_FOR_WRITER)
                else
                {
                    printf("\n--You have a mistake here: rythm duration should be a positive number!--\n");
                }
                #endif
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_PAUSE_S, strlen(NARRATION_TAG_PAUSE_S))==0 && !bIsSkip)
            {
                //printf("\nhere is pause s\n");
                // check if the given number is positive
                if(bIsPositiveNumber(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_S)))
                {
                    sscanf(pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), "%d", &iDelayDuration);
                    //printf("\ncmd=%s, pause duration str=%s, size=%d | pause duration=%d\n", pcNarrationTagHolder, pcNarrationTagHolder+strlen(NARRATION_TAG_PAUSE_S), strlen(pcNarrationTagHolder)-strlen(NARRATION_TAG_PAUSE_S), iDelayDuration);
                    delay_ms(iDelayDuration*1000);
                }
                #if defined(BUILD_FOR_WRITER)
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
                #if defined(BUILD_FOR_WRITER)
                else
                {
                    printf("\n--You have a mistake here: pause duration should be a positive number!--\n");
                }
                #endif
            }
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_SKIP_REQUEST_YES, strlen(NARRATION_TAG_SKIP_REQUEST_YES))==0 && !bIsSkip)
            {
                //printf("\nask skip\n");
                if(bYesNoQuestion(pcNarrationTagHolder+strlen(NARRATION_TAG_SKIP_REQUEST_YES)))
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
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_SKIP_REQUEST_NO, strlen(NARRATION_TAG_SKIP_REQUEST_NO))==0 && !bIsSkip)
            {
                //printf("\nask skip\n");
                if(bYesNoQuestion(pcNarrationTagHolder+strlen(NARRATION_TAG_SKIP_REQUEST_NO)))
                {
                    //printf("\nskip no\n");
                    bIsSkip = false;
                }
                else
                {
                    //printf("\nskip yes\n");
                    bIsSkip = true;
                }
            }
            else if(strcmp(pcNarrationTagHolder, NARRATION_TAG_SKIP_END)==0)
            {
                //printf("\nend skip\n");
                bIsSkip = false;
            }
            // default text style
            else if(strcmp(pcNarrationTagHolder, NARRATION_TAG_TEXT_STYLE_DEFAULT)==0)
            {
                //printf("\nset default text style\n");
                vSetTextSyleToDefault();
            }
            // color
else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_TEXT_COLOR, NARRATION_TAG_TEXT_COLOR_SIZE)==0)
            {
                for(iIterator=0; iIterator<c_maxColorNbr; iIterator++)
                {
                    if(strcmp(pcNarrationTagHolder+NARRATION_TAG_TEXT_COLOR_SIZE, TextColors_tags[iIterator])==0)
                    {
                        vSetTextColor(iIterator);
                        break;
                    }
                }
                #if defined(BUILD_FOR_WRITER)
                if(iIterator==c_maxColorNbr)
                {
                    printf("\n--You have a mistake here: uncorrect color!--\n");
                }
                #endif
            }
            // Font
            else if(strncmp(pcNarrationTagHolder, NARRATION_TAG_TEXT_FONT, NARRATION_TAG_TEXT_FONT_SIZE)==0)
            {
                for(iIterator=0; iIterator<f_maxFontNbr; iIterator++)
                {
                    if(strcmp(pcNarrationTagHolder+NARRATION_TAG_TEXT_FONT_SIZE, TextFonts_tags[iIterator])==0)
                    {
                        vSetTextFont(iIterator);
                        break;
                    }
                }
                #if defined(BUILD_FOR_WRITER)
                if(iIterator==f_maxFontNbr)
                {
                    printf("\n--You have a mistake here: uncorrect font!--\n");
                }
                #endif
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
            #if defined(BUILD_FOR_WRITER)
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
                iDelayDuration = iRythmDuration;
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