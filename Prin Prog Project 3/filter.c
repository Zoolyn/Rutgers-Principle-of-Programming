/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: filter.c                            *
 *  Date: 11/25/2018                          *
 **********************************************
 */

#include "filter.h"

void count_unmatched_vertices(int threadId, int threadNum, int nodeNum,
                              int *nodeToProcess, int *res,
                              int *nodeCount)
{
    /* Your Code Goes Here */
	int workchunk, beg, end;
        int i,v;	
	nodeCount[threadId] = 0;
	
        workchunk = (nodeNum + threadNum - 1)/threadNum;
        beg = threadId * workchunk;
        if(beg + workchunk <= nodeNum)
                end = beg + workchunk;
        else
                end = nodeNum;
	
	//printf("\nthreadId: %d\n", threadId);


        for(i = beg;i <= (end - 1); i++){
		v = nodeToProcess[i];
		if(res[v] == UNMATCHED){
			nodeCount[threadId]++;
		}

	}
	//printf("Node Counter:");
	//for(i=0; i<threadNum;i++){
	//	printf("%d ", nodeCount[i]);
	//}

}


void update_remain_nodes_index(int threadId, int threadNum,
                               int *nodeToProcess, int *startLocations,
                               int *res,
                               int nodeNum, int *newNodeToProcess)
{
    /* Your Code Goes Here */
	int workchunk, beg, end;
        int i,v,offset;

        workchunk = (nodeNum + threadNum - 1)/threadNum;
        beg = threadId * workchunk;
        if(beg + workchunk <= nodeNum)
                end = beg + workchunk;
        else
                end = nodeNum;


	for(i = beg;i <= (end - 1); i++){
		v = nodeToProcess[i];            
		if(res[v] == UNMATCHED){
			offset = startLocations[threadId]++;
			newNodeToProcess[offset] = v;
		}		
		
	}
}
