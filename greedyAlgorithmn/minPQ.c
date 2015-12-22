//Yong Ning Hu, direct translation of text java psuedo code 

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "minPQ.h"
#include "greedy04.h"

MinPQ create(int n, int *status, int *parent, double *fringeWgt)
	{
	MinPQ pq = malloc(sizeof(MinPQ));
	assert(pq != NULL);
	pq->parent = parent;
	pq->fringeWgt = fringeWgt;
	pq->status = status;
	for( int i = 1; i <= n; i++) {pq->status[i] = unseen;}
	pq->numVertices = n;
	pq->numPQ = 0;
	pq->minVertex = -1;
	pq->oo = INFINITY;
	return pq;
	}

void insert(MinPQ pq, int v, int newPar, double newW)
	{
	pq->parent[v] = newPar;
	pq->fringeWgt[v] = newW;
	pq->status[v] = fringe;
	pq->minVertex = -1;
	pq->numPQ++;
	}

void decreaseKey(MinPQ pq, int v, int newPar, double newW)
	{
	pq->parent[v] = newPar;
	pq->fringeWgt[v] = newW;
	pq->minVertex = -1;
	}

void deleteMin(MinPQ pq)
	{
	int oldMin = getMin(pq);	
	pq->status[oldMin] = tree;
	pq->minVertex = -1;
	pq->numPQ--;
	}

bool isEmpty(MinPQ pq)
	{
	return (pq->numPQ == 0);
	}

double getPriority(MinPQ pq, int v)
	{
	return pq->fringeWgt[v];
	}

int getMin(MinPQ pq)
	{
	if(pq->minVertex == -1) {findMin(pq);}
	return pq->minVertex;
	}

void findMin(MinPQ pq)
	{
	int v;
	double minWgt;
	
	minWgt = pq->oo;
	for(v = 1; v <= pq->numVertices; v++)
		{
		if(pq->status[v] == fringe)
			{
			if(pq->fringeWgt[v] < minWgt)
				{
				pq->minVertex = v;
				minWgt = pq->fringeWgt[v];
				}
			}
		}
	}





