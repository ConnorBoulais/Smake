# SMAKE Overview

Smake is a C program that implements a simplified version of the popular
building utility "make". Smake reads from a simple file composed only of rules,
and therefore no variables. It will build a program files specified by the rules,
only building those that are out of date.

For Smakefile format instructions, see UsageInstructions.txt

## To Build
Run "make" with Makefile, all c files, and all h files in current directory.

## To Run
- Run "./smake" to build from Smakefile in your current working directory.
- The program will build from the first rule in Smakefile by default
- Optionally takes one command-line argument, which is to be the name of the target
of a rule to start building from.

## Brief Program Walkthrough

1) Check for existence of file, and validate command-line argument (if given).
2) Parse file and assemble rules into data structures
3) Begin evaluating rules
    - Check if dependencies are files or targets of other rules
    - Recursively call on dependency if the dependency is a target of another rule.
    - Apply actions if dependencies are files and are out of date

## Data Structures

1) A Node struct will be used to store a rule. It will have the following attributes:
    - char *target -- pointer to target string
    - char **deps -- array of char pointers, where each char pointer points to an dependency string.
    - char **actions -- array of char pointers, where each char pointer points to an action string.
    - int numDeps -- int holding the number of dependencies in the dependencies array.
    - int sizeOfDeps -- int holding the size of the dependencies array -- needed so the array can be resized when necessary.
    - int numActions -- int holding the number of actions in the actions array.
    - int sizeOfActions -- int holding the size of the actions array -- needed so the array can be resized when necessary.

2) Hash Table of Nodes will be used to access and check existence of Nodes.
    - Keys will be char pointers to a string containing the target of a rule.
    - Values will be the Nodes corresponding to each target string.
    - Will keep track of table size and number of elements in table. 

## Necessary Functions    

### Smake Logic

`int isTarget(char *dependency, HashTable *hashtable)`

- This function will determine if a dependency is also a target of another rule.
- Takes a pointer to a string representing the dependency, and pointer to the hash table.
- Determines if the given dependency string has an entry in the hash table
    - If there is an entry corresponding to that dependency, then the dependency is also a target (returns 1).
    - If there is no such entry, then the dependency should be a file (assuming that file exists) (returns 0).
- Testing:
    - Test with dependencies that are targets and dependencies that are not.

`void runActions(char *actions)`

- Runs the actions specified in the string pointed to by actions.
- Prints the actions to be run prior to running them

`void evaluateRule(HashTable *hashtable, char *target)`

- This is the recursive function that will be applied to each rule, after the hashtable has been constructed.
- It is given a pointer to the hash table, and a pointer to the target string of the rule. This string will be
used to get the Node corresponding to the current rule being evaluated from the hash table.
- If no command-line arguments are given, this function is passed the target of the first rule. Otherwise,
it is passed the target specified by the single command-line argument.
- Loops through each dependency of a rule:
    - if isTarget(dependency) returns true, then recursively call this function with dependency as target,
    like so: evaluateRule(hashtable, dependency)
    - Otherwise, use validFile(dependency) to check if the dependency represents a valid file.
    - Then, use outOfDate(target, dependency) to check if this dependency needs to be updated.
        - If the dependency is out of date, call runActions(actions) to update it.
    - Go to next dependency
- Testing:
    - Test that actions are being run if and only if their dependencies are out of date.
    - This will likely be one of the last functions to be written, as it essentially defines the logic that calls
    all the other functions in this project. Therefore, it will likely be tested with an almost complete program.

### Node

`Node *createNode())`

- Mallocs a Node struct and returns a pointer to the node.
- Initializes target to NULL.
- Mallocs space for the actions array and dependencies array.

`void addAction(Node *node, char *action)`

- Adds the string pointed to by "action" to the array of actions in the "node" struct.
- Will resize the node's array if necessary.
- Testing
    - Test with large numbers of actions.

`void addDependency(Node *node, char *dependency)`

- Adds the string pointed to by "dependency" to the array of dependencies in the "node" struct.
- Will resize the node's array if necessary.
- Testing
    - Test with large numbers of dependencies.

`void freeNode(Node *node)`

- Free's a node pointed to by "node".
- Must free each attribute first, then free whole node.
- Since actions and dependencies are arrays of char pointers, will need to loop through the arrays and free
each char pointer individually.

### Hash Table

`HashTable buildHashTable(FILE *smakeFile)`

- This function will create the hash table that stores the rule Nodes for each rule in the makefile.
- Keys and Values will be as defined above.
- Will parse file and: 
    - verify file is in a correct format
    - creates Node for each rule using createNode()
    - uses strtok to add target, dependencies, and actions to node using addAction() and addDependency().
    - inserts node into the hash table.
- Takes a FILE pointer to the Smakefile.
- Testing:
    - Test with different makefiles to ensure table is constructed correctly.
    - Test with invalid makefiles to ensure function error exits properly.
    - Test ability to construct and teardown hashtable without memory leaks.

`unsigned hash(HashTable *hashtable, char *target)`

- Takes a string pointed to by target and produces an unsigned integer hash value.
- Takes in a pointer to the hash table so that it can perform a modulus with the current table size.
- Will likely start with a simple hash algorthim for strings, and then experiment with some that seem
to work better.
- Likely will want something that takes into account the order of letters. For example, simply adding up
the ascii codes for each character would map "abc" and "cba" to same value, so something more effective
is desirable.
-Testing:
    - Test with large amounts of input strings and count how many collisions occur (how many different
    strings map to the same integer).

`void freeTable(HashTable *hashtable)`

- Free's every node in the hash table.
- Testing:
    - Test with large tables
    - Test with node's that are incomplete (contain NULL pointers).

```
Node *get(HashTable *hashtable, char *target)
void resize(HashTable *hashtable)
void insert(HashTable *hashtable, Node *node)
HashTable createHashTable()
```

- Basic hash table functions will be needed to interface with the hash table.
- createHashTable() allocates the struct and initializes its pointers to NULL.
- Testing:
    - Test with NULL nodes and NULL table.
    - Test with large numbers of items to ensure inserting and resizing is done correctly.

### File I/O

`FILE *getMakeFile()`

- Looks in the current directory for smakeFile to build from.
- Returns a FILE pointer to the smakeFile.
- Error exits if no such file exists.
- Testing:
    - Test with both "smakefile" and "Smakefile" as file name.

`int validFile(char *dependency)`

- Checks if the file represented by the given FILE pointer is valid and can be opened by the program.
- Will be called on a dependency if that dependency is found to not be a target.
- Returns 1 if valid, 0 if not valid.
- Will error exit with message if file is not valid.

`int outOfDate(char *target, char *dependency)`

- This function checks if the time stamp on the file corresponding to the dependency string is more
more recent than that of the target string.
- This will only be called once it is determined that the dependency is not a target but just a file.
- Takes a pointer to the target and dependency strings.
- Returns 1 if the dependency is out of date, 0 if updated.
- Will need stat() function to get timestamp on files.
- Testing:
    - Test with files that are out of date and files that are updated.

### Utility

```
void *checkedMalloc(int size)
void *checkedCalloc(int numElements, int size)
void *checkedRealloc(void *ptr, int size)
void errorExit(char *message)
```
- Used to reduce code duplication
- Checked allocation functions add error checking
- errorExit() prints a given message and then exits the program with an exit status.
