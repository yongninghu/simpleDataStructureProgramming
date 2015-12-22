//Yong Ning Hu

/* inputLib.h
 * opens file, gets first line of file
 */

#ifndef C101InputLib
#define C101InputLib

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* opens file
 */
FILE *cfopen(char *file);

/* gets first line of the file
 */
char *cgetline(FILE *file);

#endif
