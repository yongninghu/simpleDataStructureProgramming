//Yong Ning Hu

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "inputLib.h"
#include "greedy04.h"
#include "minPQ.h"
#include "edgeList.h"
#include "loadWgtGraph.h"

/* Main
 * Creates a EdgeList array with file data.
 * Implements Prim's Minimum Spanning Tree Algorithm
 * It will also execute Dijkstra's Shortest-Path algorithm
 * Output the arrays filled by the algorithm. Show columns 
 * for each vertex number, status, parent, priority (cost,
 * depending on which algorithm). Print a heading that
 * states which algorithm was run and what the start vertex 
 * is.
 */

int Exit_Status = 0;

int main (int argc, char **argv)
	{
	bool prim = false;
	int svert = -1;
	int m, n = 0;
	if(argc != 4)
		{
		printUsage();
		return Exit_Status;
		}
	for(int i = 1; i < argc-1; i++)
		{
		int ref;
		char *command = argv[i];
		if((command[0] == '-' && (int)strlen(command) > 1))
			{
				if(strcmp(command, "-D") == 0)
					{
					continue;
					}
				if(strcmp(command, "-P") == 0)
					{
					prim = true;
					continue;
					}
			}
		if(sscanf(argv[i], "%d", &ref) != 0 )
			{
				svert = ref;
				continue;
			}
		printUsage();
		Exit_Status = 1;
		return Exit_Status;
		}
 
        FILE *file = cfopen(argv[3]);
	
	if(file == NULL)
		{
                fflush(NULL);
                fprintf(stderr, "%s (%s)\n", argv[3],
                        "No such file or directory");
                fflush(NULL);
                Exit_Status = 1;
		return Exit_Status;
		}
		
	printf("Opened %s for input\n", argv[3]);

	char *line = cgetline(file);
	n = parseN(line);
	int *status = calloc(n+1, sizeof(int));
	int *parent = calloc(n+1, sizeof(int));
	double *fringeWgt = calloc(n+1, sizeof(double));
	
	EdgeList * adjInfo = initEdges(n);
	m = loadEdges(file, adjInfo, n, prim);
	
	if(svert > n || 0 > svert) 
		{
		printf("%s\n", "starting vertex is out of range");
		printUsage();
		return Exit_Status;
		}
	
	if(prim == true)
		{
		printf("%s, starting vertex: %d\n", "Prim's algorithm", svert);	
	} else  printf("%s, starting vertex: %d\n", "Dijkstra's algorithm", svert);
	
	greedyTree(adjInfo, n, svert, status, parent, fringeWgt, prim);
	
	printOutput(n, status, parent, fringeWgt);

	return Exit_Status;
	}

void greedyTree(EdgeList *adjInfo, int n, int svert, int *status, int *parent, double *fringeWgt, bool prim)
	{
	MinPQ pq = create(n, status, parent, fringeWgt);

	insert(pq, svert, -1, 0);
	while(isEmpty(pq) == false)
		{
		int v = calcPriority(pq);
		updateFringe(pq, adjInfo[v], v, prim);
		}
	}

void updateFringe(MinPQ pq, EdgeList adjInfoOfV, int v, bool prim)
	{
	double dijDist = pq->fringeWgt[v];   
	EdgeList remAdj = adjInfoOfV;
	while(remAdj != edgeNil)
		{
		EdgeInfo wInfo = edgeFirst(remAdj);
		int w = wInfo.to;
		double newWgt = (prim ? 0 : dijDist) + wInfo.wgt;
		if(pq->status[w] == unseen)
			insert(pq, w, v, newWgt);
		else if(pq->status[w] == fringe)
			{
			if(newWgt < getPriority(pq,w))
				{
				decreaseKey(pq, w, v, newWgt);
				}	
			}
		remAdj = edgeRest(remAdj);
		}
	}

int calcPriority(MinPQ pq)
	{
	int v = getMin(pq);
	deleteMin(pq);
	return v;
	}

void printOutput(int n, int *status, int *parent, double *fringeWgt)
	{
        for(int i = 1; i <= n; i++)
                {
		printf("%d   ", i);
		if(status[i] == unseen)
			{
				printf("%s\n", "status_unseen, unreachable vertex"); 
			}
		if(status[i] == tree)
			{
				printf("status_intree   parent_%d   cost_%.2f\n", parent[i], fringeWgt[i]);
			}
                }
	}

void printUsage(void)
	{
	printf("%s\n", "Usage: greedy04 -D/-P v file");
        printf("%s\n", "-D for Dijkstra's algorithm, -P for Prim's algorithm, v is starting vertex/index and it must be within 1 and n");
	}
