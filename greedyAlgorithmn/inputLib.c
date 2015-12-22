//Yong Ning Hu
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "inputLib.h"

//opens file or opens stdin for data
FILE *cfopen(char *file)
	{
	FILE* tmp = NULL;
	if(strcmp(file, "-") == 0)
                tmp = stdin;
        else
                tmp = fopen(file, "r");

	return tmp;
	}

//gets the first line of the file/stdin
char *cgetline(FILE *file)
	{
	char inbuff[2000];
	char *line = NULL;
	line = fgets(inbuff, sizeof inbuff, file);

	if(line != NULL)
		line = strdup(inbuff);

	return line;
	}
