/* Connor Boulais
 * Planck
 * CPE-357-06
 * 
 * Hash Table implementation for assignment #3.
 */

#include "HashTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_TABLE_SIZE 256
#define INITIAL_ACTIONS_SIZE 64
#define INITIAL_DEPS_SIZE 64

Node *createNode()
{
    Node *node;
    node = (Node *)checkedMalloc(sizeof(Node));
    node->target = NULL;

    node->numActions = 0;
    node->sizeOfActions = INITIAL_ACTIONS_SIZE;
    node->actions = (char **)checkedMalloc((node->sizeOfActions) * sizeof(char *));

    node->numDeps = 0;
    node->sizeOfDeps = INITIAL_DEPS_SIZE;
    node->deps = (char **)checkedMalloc((node->sizeOfDeps) * sizeof(char *));

    return node;
}

void addAction(Node *node, char *action)
{   
    if(node->numActions == node->sizeOfActions){
        node->sizeOfActions *= 2;
        node->actions = (char **)checkedRealloc(node->actions, (node->sizeOfActions) * sizeof(char *));
    }

    node->actions[node->numActions] = action;
    node->numActions += 1;
}

void addDependency(Node *node, char *dependency)
{   
    if(node->numDeps == node->sizeOfDeps){
        node->sizeOfDeps *= 2;
        node->deps = (char **)checkedRealloc(node->deps, (node->sizeOfDeps) * sizeof(char *));
    }

    node->deps[node->numDeps] = dependency;
    node->numDeps += 1;
}

void errorExit(char *message)
{
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

void perrorExit(char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
}


void *checkedMalloc(int size)
{
    void *temp = NULL;
    temp = malloc(size);
    if(temp == NULL){
        perrorExit("malloc out of memory");
    }
    return temp;
}

void *checkedCalloc(int numElements, int size)
{
    void *temp = NULL;
    temp = calloc(numElements, size);
    if(temp == NULL){
        perrorExit("calloc out of memory");
    }
    return temp;
}

void *checkedRealloc(void *ptr, int size)
{
    void *temp = NULL;
    temp = realloc(ptr, size);
    if(temp == NULL){
        perrorExit("realloc out of memory");
    }
    return temp;
}

void freeNodeActions(Node *node)
{
    int i;

    for(i = 0; i < node->numActions; i++){
        free(node->actions[i]);
    }
    free(node->actions);
}

void freeNode(Node *node)
{
    /* target string and dependency string are apart of same malloc block, so
    freeing target will free the strings in the deps array also. */
    if(node->target){
        free(node->target);
    }
    if(node->deps){
        free(node->deps);
    }
    if(node->actions){
        freeNodeActions(node);
    }
    free(node);
}

HashTable createHashTable()
{
    HashTable hashtable;
    hashtable.table = checkedCalloc(INITIAL_TABLE_SIZE, sizeof(Node *));
    hashtable.size = INITIAL_TABLE_SIZE;
    hashtable.numElements = 0;
    return hashtable;
}

unsigned hash(HashTable *hashtable, char *target)
{
    unsigned hash, i;

    if(!target){
        return 0;
    }

    for(hash = 0, i = 0; target[i] != '\0'; i++){
        hash = target[i] + hash*31;
    }

    return hash % hashtable->size;
}


Node *get(HashTable *hashtable, char *target)
{
    int hashVal;
    Node *temp = NULL;

    if(!hashtable || !target) {errorExit("Error: 1 or more NULL arguments to get().\n");}

    hashVal = hash(hashtable, target);

    temp = hashtable->table[hashVal];

    while(temp){
        if(!strcmp(temp->target, target)){
            return temp;
        }
        hashVal += 1;
        hashVal %= hashtable->size;
        temp = hashtable->table[hashVal];
    }
    return NULL;
}

void resize(HashTable *hashtable)
{
    int i;
    Node **newTable = NULL;

    newTable = checkedCalloc(hashtable->size * 2, sizeof(Node *));

    for(i = 0; i < hashtable->size; i++){
        newTable[i] = hashtable->table[i];
    }
    hashtable->size *= 2;

    free(hashtable->table);
    hashtable->table = newTable;
}

void insert(HashTable *hashtable, Node *node)
{
    int hashVal;

    if(hashtable->numElements > hashtable->size*3/4){
        resize(hashtable);
    }

    hashVal = hash(hashtable, node->target) % hashtable->size;

    while(hashtable->table[hashVal]){
        hashVal += 1;
        hashVal %= hashtable->size;
    }

    hashtable->table[hashVal] = node;
    hashtable->numElements += 1;
}

void freeTable(HashTable *hashtable)
{
    int i;
    Node *node = NULL;

    for(i = 0; i < hashtable->size; i++){
        node = hashtable->table[i];
        if(node){
            freeNode(node);
        }
    }
    free(hashtable->table);
}