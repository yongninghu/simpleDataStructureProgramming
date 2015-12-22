//Yong Ning Hu

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "edgeList.h"

const EdgeList edgeNil = NULL;

struct EdgeListNode
	{
	EdgeInfo info;
	EdgeList next;
	};

EdgeInfo edgeFirst(EdgeList oldL)
	{
	return oldL->info;
	}

EdgeList edgeRest(EdgeList oldL)
	{
	return oldL->next;
	}

EdgeList edgeCons(EdgeInfo newE, EdgeList oldL)
	{
	EdgeList newL = malloc( sizeof (EdgeList));
	assert (newL != edgeNil);
	newL->info = newE;
	newL->next = oldL;
	return newL;
	}











