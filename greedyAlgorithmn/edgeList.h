/* edgeList.h
 * (what is the purpose of this file?)
 * ADT for EdgeList
 * Can be used to construct an array of linked EdgeList
 * EdgeInfo is not ADT
 */

#ifndef C101EdgeList
#define C101EdgeList
/* Multiple typedefs for the same type are an error in C. */

typedef struct EdgeListNode * EdgeList;

typedef struct EdgeInfoStruct
    {
    int to;
    double wgt;
    }
    EdgeInfo;

/** edgeNil denotes the empty EdgeList */
extern const EdgeList edgeNil;

/* Access functions
 * (what are the preconditions?)
 * oldL must not be NULL
 */

/** first
 * returns EdgeInfo stored in OldL
*/
EdgeInfo edgeFirst(EdgeList oldL);

/** rest
 * rethrns the EdgeList linked to oldL
*/
EdgeList edgeRest(EdgeList oldL);

/* Constructors
 * (what are the preconditions and postconditions?)
 * prec: newE must be a valid EdgeInfo
 * postc: appends a link with newE to oldL if oldL is 
 * not NULL. If oldL is NULL, it creates a EdgeList 
 * at current index.
 */

/** cons
 * constructs a EdgeList linked to oldL or creates an
 * EdgeList at current index and link the existing EdgeLists 
 * to it.
*/
EdgeList edgeCons(EdgeInfo newE, EdgeList oldL);

#endif

