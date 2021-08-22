/* Connor Boulais
 * Planck
 * CPE-357-06
 * 
 * Functions for smake (assignment #3).
 */

#include "HashTable.h"
#include "smakeFuncs.h"
#include "readline.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdlib.h>


char *getNextNonWhiteSpace(char *line)
{
    while(*line == ' ' || *line == '\t'){
        line++;
    }
    return line;
}

char *trimWhiteSpace(char *line)
{
    char *newLine = NULL;

    if(*line == ' ' || *line == '\t'){
        newLine = checkedMalloc((strlen(getNextNonWhiteSpace(line)) + 1) * sizeof(char));
        strcpy(newLine, getNextNonWhiteSpace(line));
        free(line);
        line = newLine;
    }
    return line;
}

char *getNextLine(FILE *file, int *lineNum)
{
    char *line = NULL;

    line = readline(file);
    *lineNum += 1;

    while(line && (line[0] == '\0' || *getNextNonWhiteSpace(line) == '\0')){
        free(line);
        line = readline(file);
        *lineNum += 1;
    }
    return line;
}

void verifySeparator(char *line, int *lineNum)
{
    while(*line != '\0'){
        if(*line == ':'){
            *line = ' ';
            return;
        }
        line++;
    }
    fprintf(stderr, "Smakefile:%d: *** missing separator.  Stop.\n", *lineNum);
    exit(EXIT_FAILURE);
}

void parseTargetAndDeps(Node *node, char *line, int *lineNum)
{
    char *ptr = NULL;

    /* verify ':' separator and replace with a ' ' */
    verifySeparator(line, lineNum);

    /* Get target */
    ptr = strtok(line, ": ");
    node->target = line;

    /* start getting dependencies */
    ptr = strtok(NULL, " \t");

    /* if no dependencies, free the deps array and set it to NULL */
    if(!ptr){
        free(node->deps);
        node->deps = NULL;
        return;
    }

    while(ptr){
        addDependency(node, ptr);
        ptr = strtok(NULL, " ");
    }
}

char *parseActions(FILE *file, Node *node, char *line, int *lineNum)
{

    if(!line || line[0] != '\t'){
        free(node->actions);
        node->actions  = NULL;
        return line;
    }

    while(line && line[0] == '\t'){
        addAction(node, line);
        line = getNextLine(file, lineNum);
    }
    return line;
}

/* create the hash table that stores the rule Nodes for each rule in the makefile. 
 *  - Returns: Node pointer to the node holding the first rule in the file. */
Node *buildHashTable(HashTable *hashtable, FILE *smakeFile)
{
    char *line = NULL;
    Node *node = NULL, *first = NULL;
    int lineNum = 0;

    line = getNextLine(smakeFile, &lineNum);

    if(!line) {return NULL;}

    first = createNode();

    /* Handles if first line of file is a list of actions with no target. */
    if(*line == '\t'){
        fprintf(stderr, "Smakefile:%d: *** recipe commences before first target.  Stop.\n", lineNum);
        exit(EXIT_FAILURE);
    }
    else{
        line = trimWhiteSpace(line);
        parseTargetAndDeps(first, line, &lineNum);
        line = getNextLine(smakeFile, &lineNum);   
    }

    line = parseActions(smakeFile, first, line, &lineNum);

    insert(hashtable, first);

    while(line){

        node = createNode();

        line = trimWhiteSpace(line);

        parseTargetAndDeps(node, line, &lineNum);

        line = getNextLine(smakeFile, &lineNum);

        line = parseActions(smakeFile, node, line, &lineNum);

        insert(hashtable, node);
    }
    return first;
}

/* Checks if the file represented by the given FILE pointer is 
 * valid and can be opened by the program. 
 *  - Returns: 1 if valid, 0 if not valid. */
int validFile(char *dependency)
{
    int fd;

    if((fd = open(dependency, O_RDONLY)) < 0){
        return 0;
    }
    close(fd);
    return 1;
}

/* checks if the time stamp on the file corresponding to the dependency string is more
 * more recent than that of the target string.
 *  - Returns: 1 if the dependency is out of date, 0 if updated.*/
int outOfDate(char *target, char *dependency)
{
    struct stat buf1;
    struct stat buf2;

    if(!target || !dependency) {errorExit("Attempting to compare timestamps of NULL pointer(s).\n");}

    if(!validFile(dependency)){
        fprintf(stderr, "smake: *** No rule to make target `%s', needed by `%s'. Stop.\n", dependency, target);
        exit(EXIT_FAILURE);
    }

    if(!validFile(target)) {return 1;}

    if(stat(target, &buf1) < 0){printf("%s\n", target); perrorExit("stat failure");}

    if(stat(dependency, &buf2) < 0){printf("%s\n", dependency); perrorExit("stat failure");}

    return (buf2.st_mtime > buf1.st_mtime);
}

int runActions(Node *node)
{
    int i, code;
    char *ptr = NULL;

    if(node->actions == NULL){
        return 0;
    }

    for(i = 0; i < node->numActions; i++){

        ptr = getNextNonWhiteSpace(node->actions[i]);

        printf("%s\n", ptr);

        code = system(ptr);
        
        if(code == -1){
            perrorExit("error applying actions");
        }
        if(code != 0){
            fprintf(stderr, "smake: *** [%s] Error\n", node->target);
            exit(EXIT_FAILURE);
        }
    }
    return 1;
}

int evaluateRule(HashTable *hashtable, Node *current)
{
    int i, needToUpdate = 0;
    char *curDep = NULL;
    Node *next = NULL;

    if(current->deps == NULL){
        return runActions(current);
    }

    for(i = 0; i < current->numDeps; i++){

        curDep = current->deps[i];

        if((next = get(hashtable, curDep))){
            needToUpdate |= evaluateRule(hashtable, next);
        }
        else{
            needToUpdate |= outOfDate(current->target, curDep);
        }
    }
    if(needToUpdate){
        runActions(current);
    }
    return needToUpdate;
}

void exitWithFree(HashTable *hashtable, FILE *file)
{
    freeTable(hashtable);
    fclose(file);
}