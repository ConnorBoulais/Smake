/* Connor Boulais
 * Planck
 * CPE-357-06
 * 
 * Hash Table header file for assignment #3.
 */

#ifndef HASH_H
#define HASH_H

typedef struct Node{
    char *target;
    char **deps;
    char **actions;
    int numActions;
    int sizeOfActions;
    int numDeps;
    int sizeOfDeps;
} Node;

typedef struct HashTable{
    Node **table;
    int size;
    int numElements;
} HashTable;

HashTable createHashTable();
unsigned hash(HashTable *, char *);
Node *get(HashTable *, char *);
void resize(HashTable *);
void insert(HashTable *, Node *);
void freeTable(HashTable *);
Node *createNode();
void addAction(Node *, char *);
void addDependency(Node *, char *);
void errorExit(char *);
void perrorExit(char *);
void *checkedMalloc(int);
void *checkedCalloc(int, int);
void *checkedRealloc(void *, int);
void freeNode(Node *);

#endif