/* Connor Boulais
 * CPE-357-06
 * Planck
 *
 * Header file for mytrFuncs.c, storing functions
 * for mytr.c
 */


void initializeLetters(char [], int);
void setLetters(char [], char *, char *);
int deleteModeCheck(char *[]);
int verifyDeleteArgs(char *[], int);
void runDelete(char *);
void runTranslate(char *, char *);
char *parseSetArg(char *);
char getEscapeChar(char);
char *extendSet2(char *, char *);
