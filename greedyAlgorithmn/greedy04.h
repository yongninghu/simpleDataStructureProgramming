//Yong Ning Hu

#ifndef CMPS101Greedy04
#define CMPS101Greedy04

#define unseen (0)
#define fringe (1)
#define tree (2)

#include "minPQ.h"
#include "edgeList.h"

//constructs tree with priority queue
void greedyTree(EdgeList *adjInfo, int n, int svert, int *status, int *parent, double *fringeWgt, bool prim);

//updates fringe of new tree edges
void updateFringe(MinPQ pq, EdgeList adjInfoOfV, int v, bool prim);

//calculates priority of priority queue and returns the index of minimum cost vertex
int calcPriority(MinPQ pq);

//prints out the final result
void printOutput(int n, int *status, int *parent, double *fringeWgt);

//prints usage of the program
void printUsage(void);

#endif
