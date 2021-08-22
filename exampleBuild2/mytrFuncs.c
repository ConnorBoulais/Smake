/*Connor Boulais
 * CPE-357-06
 * Planck
 *
 * Functions for mytr.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Initializes array of 256 where index represents ascii value of a
 * character, and the value at that index is the translated version
 * of that character*/
void initializeLetters(char letters[], int size)
{
    int i;

    for(i = 0; i < size; i++){
        letters[i] = i;
    }
}



/* Puts the translated characters into their spots in the 
 * letters array.*/
void setLetters(char letters[], char *set1, char *set2)
{
    int i;

    for(i = 0; set1[i] != '\0'; i++){ 
        letters[((int)set1[i])] = set2[i];
    }
}

/* checks if in delete mode. Returns:
 * 1 -> in delete mode
 * 0 -> not in delete mode */
int deleteModeCheck(char *argv[], int argc)
{
    if(argv[1][0] == '-'){
        return 1;
    }

    return 0;
}

/* Verifies delete arguments
   Returns int representing the index in the arguments
   of the set containing the characters to delete*/
int verifyDeleteArgs(char *argv[], int argc)
{
    int i, j, len;

    /*
    -Since actual tr treats -dddd the same as -d, must check to see if 
    the delete flag was repeated
    -tr also treats '-dx' the same as '-d -x', and therefore reports an error
    as there is no '-x' flag, so mytr will do the same.
    -tr also treats '-d -d -d' as '-d', so need to handle that.
    */

    for(j = 1; j < argc && argv[j][0] == '-'; j++){

        len = strlen(argv[j]);

        for(i = 1; i < len; i++){
            if(argv[j][i] != 'd'){
                fprintf(stderr, "mytr: invalid option -- '%c'\n", argv[j][i]);
                exit(EXIT_FAILURE);
            }
        }
    }

    if((argc - j) < 1){
        fprintf(stderr, "mytr: missing operand\n");
        exit(EXIT_FAILURE);
    }
    else if((argc - j) > 1){
        fprintf(stderr, "mytr: extra operand '%s'\n", argv[j+1]);
        exit(EXIT_FAILURE);
    }

    return j;
}

/* deletes characters in set1 from stdin and outputs
 * remaining characters to stdout */
void runDelete(char *set1)
{
    int c;

    while((c = getchar()) != EOF){
        
        if(strchr(set1, c) == NULL){
            putchar(c);
        }
    }
    free(set1);
}

/* runs translate given a complete set1 of characters to
 * be replaced with a given comeplete set2 of characters.*/
void runTranslate(char *set1, char *set2)
{
    int c;
    char letters[255];

    /*initialize letters array*/
    initializeLetters(letters, 256);
    
    /*set letters to be translated*/
    setLetters(letters, set1, set2);

    while((c = getchar()) != EOF){
        putchar(letters[c]);
    }
    free(set1);
    free(set2);
}

/* takes a character, 'c', and returns the escape character 
 * represented by '\c'. */
char getEscapeChar(char c)
{
    if(c == 'n'){
        return '\n';
    }
    if(c == 't'){
        return '\t';
    }
    if(c == '\\'){
        return '\\';
    }
    if(c == '\0'){
        return '\\';
    }
    return c;
}

/* Takes a string from the command-line and returns a
 * compete string where escaped characters have their actual
 * value, and grouping quotes are removed.*/
char *parseSetArg(char *arg)
{
    int argLen, i;
    char *set, c;

    argLen = strlen(arg);
    set = (char *)malloc((argLen+1) * sizeof(char));

    /*Error check malloc*/
    if(set == NULL){
        fprintf(stderr, "Error with malloc at mytrFuncs.c:parseSetArg\n");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < argLen; i++){
        
        if(arg[i] == '\\'){
            c = getEscapeChar(arg[i+1]);
            set[i] = c;
            arg++;
        }
        else{
            set[i] = arg[i];
        }
    }
    set[i] = '\0';
    return set;
}

/* adjusts length of set2 if needed - i.e.
 * if set2 is shorter than set1. Returns a
 * pointer to set2 */
char *extendSet2(char *set1, char *set2)
{
    int set1Len, set2Len, i;
    char *extendedSet2, *temp;

    set1Len = strlen(set1);
    set2Len = strlen(set2);

    if(set1Len <= set2Len){
        return set2;
    }

    temp = (char *)realloc(set2, (set1Len + 1) * sizeof(char));

    /*Error check realloc*/
    if(temp == NULL){
        fprintf(stderr, "Error with realloc in mytrFuncs.c:extendSet2");
        free(set2);
        exit(EXIT_FAILURE);
    }
    
    extendedSet2 = temp;
    
    for(i = set2Len; i < set1Len; i++){
        extendedSet2[i] = extendedSet2[set2Len-1];
    }
    extendedSet2[i] = '\0';
    return extendedSet2;
}

