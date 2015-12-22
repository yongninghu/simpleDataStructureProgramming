//Yong Ning Hu
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "edgeList.h"
#include "loadWgtGraph.h"

struct Edge
	{
	int from;
	int to;
	double wgt;
	};

//gets n from the first line of data
int parseN(char *line)
	{
	int num = 0;
        int scanfr = 0;
        scanfr = sscanf(line, "%d", &num);
        return num;
	}

//creates a IntList array with size n + 1
EdgeList *initEdges(int n)
	{
	EdgeList * adjInfo = calloc(n + 1, sizeof (EdgeList));
        assert(adjInfo != NULL);

        for(int i = 0; i <= n; i++)
                adjInfo[i] = edgeNil;

	return adjInfo;
	}
//creates edge for IntList input with each line from file
Edge parseEdge(char *line)
        {
        int p, q = 0;
        double wgt = 0.0;
        int sscanfRetn;

        sscanfRetn = sscanf(line, " %d %d %lf ", &p, &q, &wgt);
	
	if(sscanfRetn != 3) return NULL;

        Edge e = malloc(sizeof (Edge));
	assert(e != NULL);
        e->from = p;
        e->to = q;
	e->wgt = wgt; 
        return e;
        }

//inserts edges into IntList array with file and checks if edge is in range
int loadEdges(FILE *file, EdgeList *adjInfo, int n, bool prim)
	{
	int edges = 0;
	char inbuff[2000];
	char *line;

	line = fgets(inbuff,sizeof inbuff, file);

        while(line != NULL)
                {
		Edge e = parseEdge(inbuff);
		if(e == NULL) {printf("%s\n","Bad edge, expects format int int double");}
		else if(e->from > n || e->to > n || e->from < 1 || e->to < 1)
			{
			printf("%s [%d, %d]\n","Bad edge out of range:",e->from,e->to);
		}else{
			EdgeInfo temp;
			temp.to = e->to;
			temp.wgt = e->wgt;
			adjInfo[e->from] = edgeCons(temp, adjInfo[e->from]);
			if(prim == true)
				{
				EdgeInfo undirected;
				undirected.to = e->from;
				undirected.wgt = e->wgt;
				adjInfo[e->to] = edgeCons(undirected, adjInfo[e->to]);
				}
			edges++;
			}
                line = fgets(inbuff, sizeof inbuff, file);
                }	

	return edges;
	}

