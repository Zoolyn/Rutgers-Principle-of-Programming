/*
 **********************************************
 *  CS314 Principles of Programming Languages *
 *  Fall 2018                                 *
 *  File: oneway.c                            *
 *  Date: 11/25/2018                          *
 **********************************************
 */
#include "oneway.h"
#include "utils.h"

void extend_one_hand(int threadId, int threadNum, GraphData graph,
                     int nodeNum, int *nodeToProcess,
                     int *res, int *strongNeighbor)
{
    /* Your Code Goes Here */
	int workchunk, beg, end;
	int v,i,j,x;
	
	workchunk = (nodeNum + threadNum - 1)/threadNum;
	beg = threadId * workchunk;
	if(beg + workchunk < nodeNum)
		end = beg + workchunk;
	else
		end = nodeNum;

	for(i = beg;i <= (end - 1); i++){
		v = nodeToProcess[i];
		strongNeighbor[v] = UNMATCHED;
		//printf("\nNode we are looking at: %d\n", v);	
		for(j = graph.offset[v]; j <= (graph.offset[v] + graph.degree[v] - 1);j++){
			//printf("\ncurrent neighbors :");
			for(x = graph.offset[v]; x <= (graph.offset[v] + graph.degree[v] - 1);x++){
				//printf("(%d, res: %d) ",graph.index[x], res[graph.index[x]]);
			}
			//printf("\n");
			if(res[graph.index[j]] == UNMATCHED){
				strongNeighbor[v] = graph.index[j];
				break;
			}
		}
		if(strongNeighbor[v] == UNMATCHED){
			res[v] = NO_MATCHED_NODES;
		}
		
			
	}
	//printf("\n strongNeighbor: ");
	for(i=0;i<9;i++){
		//printf("\nres: %d\n", res[i]);
		//printf("%d ",strongNeighbor[i]);
	}
	//printf("\n");

}

void check_handshaking(int threadId, int threadNum,
                       int nodeNum, int *nodeToProcess,
                       int *strongNeighbor, int *res)
{
    /* Your Code Goes Here */
    	int workchunk, beg, end;
	int i,v,s;
	
	workchunk = (nodeNum + threadNum - 1)/threadNum;
	beg = threadId * workchunk;
	if(beg + workchunk <= nodeNum)
		end = beg + workchunk;
	else
		end = nodeNum;
	for(i = beg;i <= (end - 1); i++){
		
		v = nodeToProcess[i];
		//printf("\nv2: %d\n", v);
		s = strongNeighbor[v];
		//printf("\ns: %d\n", s);
		
		if(v == strongNeighbor[s]){
			res[v] = s;
		}
		
	}

	//printf("\n res: ");
        //for(i=0;i<9;i++){
        //        printf("%d ",res[i]);
        //}
        //printf("\n");





}


