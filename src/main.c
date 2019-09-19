#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions.h"

int main()
{
    struct Graph* graph = graphConstructor();

    graphBuild(graph);

    //Test function to show the entire graph in the console
    //graphDisplay(graph);

    dijkstraRouteFromFile(graph);
    printf("\nOutput sent to 'output.txt' file");


    //Test for time taken to build graph from file
    /*int i;
    double st = (double)clock();
    for (i = 0; i <= 100; i++)
    {
        graphBuild(graph);
    }
    double et = (double)clock();
    double tt = (et - st)/CLOCKS_PER_SEC;
    printf("\nTime taken to build the graph from file 100 times: %f", tt);
    printf("\nAverage time taken to build graph from file: %f", tt/100);*/

    //Test to time how long it takes to open the file
    /*double st = (double)clock();
    dijkstraRouteFromFile(graph);
    double et = (double)clock();
    double tt = (et - st)/CLOCKS_PER_SEC;
    printf("\nTime taken to build graph from file: %f", tt);*/

    //Memory allocation for graph
    /*printf("\nsize of graph struct (c): %d", sizeof(struct Graph));
    printf("\nsize of node struct (a): %d", sizeof(struct graphNodes));
    printf("\nsize of edge struct (b): %d", sizeof(struct graphEdges));
    printf("\nsize of entire graph: be + an + c, e = num edges, n = num nodes");
    printf("\n                    : %d + %d + %d = %d bytes", sizeof(struct graphEdges)*80, sizeof(struct graphNodes)*21, sizeof(struct Graph), sizeof(struct graphEdges)*80 + sizeof(struct graphNodes)*21 + sizeof(struct Graph)); //16*80 = 1280, 40*21 = 840
    */

    return 0;
}
