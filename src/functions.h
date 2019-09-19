#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
//#include "functions.c"

struct Graph;

struct Graph* graphConstructor();

void graphBuild(struct Graph* graph);

void graphDisplay(struct Graph* graph);

void graphDijkstra(struct Graph* graph, int startNodeIndex, int endNodeIndex, FILE* outputFile);

#endif // FUNCTIONS_H_INCLUDED
