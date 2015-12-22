//Yong Ning Hu, direct translation of java pseudo code from textbook

/* minPQ.h
 * Qusi-ADT for MinPQ
 * Constructs a priority queue for calculating
 * priority of fringes
 */
#ifndef CMPS101MinPQ
#define CMPS101MinPQ

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct PQInfo
	{
	int numVertices, numPQ;
	int minVertex;
	double oo;
	int *status;
	int *parent;
	double *fringeWgt;
	} *MinPQ;

/* Constructor
 * prec: status, parent, and fringeWgt must be valid array, n must be > 1
 * postc: creates a priority queue with given arrays for priority calculation
 */

/** create
 * creates a priority queue with given arrays
 */
MinPQ create(int n, int *status, int *parent, double *fringeWgt);

/** insert
 * inserts a new queue
 */ 
void insert(MinPQ pq, int v, int newPar, double newW);

/** decreaseKey
 * updates the existing queue with new info
 */
void decreaseKey(MinPQ pq, int v, int newPar, double newW);

/** deleteMin
 * takes the queue off and set it to intree
 */
void deleteMin(MinPQ pq);

/** isEmpty
 * if no queue is left returns true
 */
bool isEmpty(MinPQ pq);

/** getPriority
 * returns priority of index v
 */
double getPriority(MinPQ pq, int v);

/** getMin
 * gets minimum cost priority and returns the index of it
 */
int getMin(MinPQ pq);

/** findMin
 * carries out search in queue for minimum cost vertex
 */
void findMin(MinPQ pq);

#endif
