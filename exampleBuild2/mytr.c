/* Connor Boulais
 * CPE-357-06
 * Planck
 *
 * ASSIGNMENT #1 mytr
 *
 * - simple implementation of unix tr command
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mytrFuncs.h"





int main(int argc, char *argv[])
{

	/* Note: set1 holds the characters to be replaced and
     * set2 holds the translations to apply*/
    int del_idx; 
    char *set1, *set2;

    if(deleteModeCheck(argv)){

        del_idx = verifyDeleteArgs(argv, argc);

        set1 = parseSetArg(argv[del_idx]);

        runDelete(set1);

        return 0;
    }

    if(argc < 3){
        fprintf(stderr, "mytr: missing operand after '%s'\n", argv[argc-1]);
        return 0;
    }
    else if(argc > 3){
        fprintf(stderr, "mytr: extra operand '%s'\n", argv[argc-1]);
        return 0;
    }

    set1 = parseSetArg(argv[1]);
    set2 = parseSetArg(argv[2]);

    set2 = extendSet2(set1, set2);

    runTranslate(set1, set2);

	return 0;
}

