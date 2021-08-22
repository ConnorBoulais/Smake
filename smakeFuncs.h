/* Connor Boulais
 * Planck
 * CPE-357-06
 * 
 * smake Functions header file for assignment #3.
 */

#ifndef SMAKEFUNCS_H
#define SMAKEFUNCS_H

#include "HashTable.h"
#include <stdio.h>

char *getNextNonWhiteSpace(char *);
char *getNextLine(FILE *, int *);
void verifySeparator(char *, int *);
void parseTargetAndDeps(Node *, char *, int *);
char *parseActions(FILE *, Node *, char *, int *);
Node *buildHashTable(HashTable *, FILE *);
int validFile(char *);
int outOfDate(char *, char *);
int runActions(Node *);
int evaluateRule(HashTable *, Node *);
void exitWithFree(HashTable *, FILE *);

#endif