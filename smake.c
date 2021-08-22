/* Connor Boulais
 * Planck
 * CPE-357-06
 * 
 * Assignment 3 - smake - Decomposition Outline
 *
 * SMAKE: this program will implement a simplified version of the popular
 * building utility "make". */

#include "smakeFuncs.h"
#include "readline.h"
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    Node *first = NULL;
    HashTable hashtable;

    hashtable = createHashTable();

    file = fopen("Smakefile", "r");
    if(!file) {
        if(argc > 1){
            fprintf(stderr, "smake: *** No rule to make target `%s'.  Stop.\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        errorExit("smake: *** No targets specified and no smakefile found.  Stop.");
    }

    if((first = buildHashTable(&hashtable, file)) == NULL){
        fprintf(stderr, "smake: *** No targets.  Stop.");
        exitWithFree(&hashtable, file);  
        return 0;
    }
    if(argc > 1){
        first = get(&hashtable, argv[1]);
        if(!first){
            fprintf(stderr, "smake: *** No rule to make target `%s'.  Stop.\n", argv[1]);
            exitWithFree(&hashtable, file);  
            return 0;
        }
    }
    if(!evaluateRule(&hashtable, first)){
        printf("smake: Nothing to be done for `%s'.\n", first->target);
    }  
    
    exitWithFree(&hashtable, file);  
    return 0;
}