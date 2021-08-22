/* Connor Boulais
   CPE-357-06
   Planck

   -Implementation of readline function
   -Will read a line of any length from a file
   -Returns a char pointer to a string containing the line
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "readline.h"

#define BUFFER_SIZE 64




/* Input: File ptr to file to read from
   Returns: char ptr to string containing the line on the heap
*/
char *readline(FILE *file)
{
	char *line = NULL;
	int c, sizeOfBlock = BUFFER_SIZE, lineCount = 0;

	if(file == NULL){
		return NULL;
	}

	c = fgetc(file);

	if(c == EOF){
		return NULL;
	}

	line = (char *)myRealloc(line, sizeOfBlock * sizeof(char));

	while((c != EOF) && (c != '\n') && !checkWindowsNewLine(c, file)){

		/*Add char to buffer*/
		line[lineCount++] = c;

		/*if buffer is full*/
		if(lineCount == sizeOfBlock){

			/*allocate more space to store line*/
			sizeOfBlock *= 2;
			line = myRealloc(line, sizeOfBlock * sizeof(char));
		}
		/*get next character*/
		c = fgetc(file);
	}

	/*resize to exact size*/
	line = myRealloc(line, (lineCount + 1)*sizeof(char));
	/*add null byte*/
	line[lineCount] = '\0';

	return line;
}

/* wrapper of realloc() funciton
 * - runs error check for readability in rest of code
 */
void *myRealloc(void *ptr, int size){

	void *temp = NULL;

	temp = realloc(ptr, size);
	if(temp == NULL){
		fprintf(stderr, "Error in realloc\n");
		exit(EXIT_FAILURE);
	}
	return temp;
}

/* Checks for windows newline - "\r\n"
 * Returns:
 * 	- 1 if reached a windows new line
 *  - 0 if not */
int checkWindowsNewLine(char c, FILE *file)
{
	if(c != '\r'){
		return 0;
	}

	c = fgetc(file);
	if(c != '\n'){
		ungetc(c, file);
		return 0;
	}
	return 1;
}