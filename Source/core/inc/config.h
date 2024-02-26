/* ----- User Configuration ----- */
// Path to the file you want to encrypte
#define RAW_FILE_NAME "../../../docs/aboutMe.txt" // path: StoryTime_Project/docs/aboutMe.txt
// Path to the encrypted file
#define ENCRYPTED_FILE_NAME "../../../docs/aboutMe_encrypted.txt" // path: StoryTime_Project/docs/aboutMe_encrypted.txt
// Terminal Titile
#define CT_TITLE "Story Time App"

#define MAX_FILE_NAME_LEN   200 // This is the max length of the encrypted and story file name and path
//#define BUILD_FOR_WRITER // use this macro to choose between building a reader or writer version (this macro is defined by the Makefile)
//#define ENABLE_TEST // use this macro to include and run your tests

#if !defined(BUILD_FOR_WRITER)
// Pre-set the Encryption key pair, set it to 0 to ignore this configuration
#define PRIVET_KEY_DECRYPTION_KEY   0  
#define PRIVET_KEY_MODULUS          0
    // #if((PRIVET_KEY_DECRYPTION_KEY<0)||(PRIVET_KEY_MODULUS<0))
    //     #error Public key required, you as a job applicant, you need to provide it. if you are the RH manager, please edit line 52-53 with correct privet key values
    // #endif
#endif

/* ----- Privet Configuration ----- */
#if defined _WIN32  || defined __CYGWIN__
#define OS_WINDOWS
#endif
// if DEBUG macro wasn't defined in the build cmd (gcc) then give it a default value
#ifndef DEBUG
#define DEBUG 0 // debug levels >= 0, 0 means hide all debug messages (this macro already handled in Makefile)
#endif