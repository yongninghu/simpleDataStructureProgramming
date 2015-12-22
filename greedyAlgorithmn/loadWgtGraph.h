/* loadGraph.h
 * header file for loadGraph.c
 * reads inputs from a file and insert inputs into a IntList
 */

#ifndef C101LoadWgtGraph
#define C101LoadWgtGraph

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "edgeList.h"

typedef struct Edge * Edge;

/* creates an intList with n nodes
 */
EdgeList *initEdges(int n);

/* gets n from the first line of the graph
 */
int parseN(char *line);

/* creates an edge with the line
 */
Edge parseEdge(char *line);

/* loads the file and creates an edge for each line
 */
int loadEdges(FILE *file, EdgeList *adjInfo, int n, bool prim);

#endif
