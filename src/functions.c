#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INFINITY 99999

//Structure for the overall graph (16bytes)
struct Graph
{
    int noEdges; //Number of edges
    int noNodes; //Number of nodes

    //pointers to head and tail of node list
    struct graphNodes* head;
    struct graphNodes* tail;
};

//Constructor for the graph
struct Graph* graphConstructor()
{
    struct Graph* graph;
    graph = malloc(sizeof(struct Graph));

    graph->noNodes = 0;
    graph->head = NULL;
    graph->tail = NULL;

    return graph;
};

//Structure for linked list of nodes (40bytes)
struct graphNodes
{
    //Char array to store city names (20 chars as this is likely to store any city name)
    char nodeName[20];

    int index;

    //Pointers to the next and previous elements in the list
    struct graphNodes* next;
    struct graphNodes* prev;

    //Pointers to the head and tail of the list of edges associated with each node
    struct graphEdges* head;
    struct graphEdges* tail;
};

//Constructor for individual nodes
struct graphNodes* nodeConstructor()
{
    struct graphNodes* node;
    node = malloc(sizeof(struct graphNodes));

    node->index = 0;
    node->next = NULL;
    node->prev = NULL;
    node->head = NULL;
    node->tail = NULL;

    return node;
};

//Structure for linked list of edges, each node having its own list (16bytes)
struct graphEdges
{
    //Variable to hold the length of an edge
    int weight;

    //Pointer to the destination of an edge
    struct graphNodes* destination;

    struct graphEdges* next;
    struct graphEdges* prev;
};

//Constructor for individual edges
struct graphEdges* edgeConstructor()
{
    //Create structure and allocate required memory
    struct graphEdges* edge;
    edge = malloc(sizeof(struct graphEdges));

    //Set the edge field to be default values
    edge->weight = 0;
    edge->destination = NULL;
    edge->next = NULL;
    edge->prev = NULL;

    return edge;
}

//Function to check a string against each node in the graph
//Used to see if a string should be added as a node or if it already exists
//Can also be used as a node index retriever
int nodeCompare(struct Graph* graph, char word[])
{
    struct graphNodes* node = graph->head;
    int i;
    for (i = 0; i < graph->noNodes; i++)
    {
        if(strcmp(node->nodeName, word) == 0) //If node is equal to word
        {
            return i; //returns the index of said node
        }
        else
        {
            node = node->next;
        }
    }
    return -1; //Node doesn't exist and needs to be added
}

//Linear search O(n) function returning pointer to the node
struct graphNodes* nodeSearch(struct Graph* graph, int index)
{
    //Temporary node structure starting from the first element in the node list
    struct graphNodes* node = graph->head;
    //Iterate through all nodes to retrieve the node with said index
    while(node != NULL)
    {
        if(node->index == index)
        {
            //Returns pointer to the desired node
            return node;
        }
        //Increment the node being compared to the next in the list
        node = node->next;
    }
    //Should never happen but for testing purposes
    return NULL;
    printf("Node %d not found", index);
}

//Function used to add a node to the graph
void graphAddNode(struct Graph* graph, char cityName[], int index)
{
    struct graphNodes* node = nodeConstructor();

    //Check to see if the node will be the first one, if so set it to be head
    if(graph->noNodes == 0)
    {
        graph->head = node;
    }
    //If there are already nodes in the graph, add new node to end of list
     if(graph->noNodes > 0)
    {
        graph->tail->next = node;
    }
    //Set the new tail to be the last element
    graph->tail = node;

    node->index = index;

    //Copy the city name into the nodes name element
    strcpy(node->nodeName, cityName);

    graph->noNodes++;
}

//Function to add an edge to the graph, to be used within graphAddEdges function
void graphAddEdge(struct Graph* graph, char startCity[], char endCity[], int weight)
{

    struct graphNodes* startNode;
    struct graphNodes* endNode;

    //Retrieve the node structures
    startNode = nodeSearch(graph, nodeCompare(graph, startCity));
    endNode = nodeSearch(graph, nodeCompare(graph, endCity));

    struct graphEdges* edge = edgeConstructor();

    //If its the nodes first edge, make it head and tail
    if(startNode->head == NULL)
    {
        startNode->head = edge;
        startNode->tail = edge;
    }
    else
    {
        startNode->tail->next = edge;
        startNode->tail = edge;
    }
    //Set the edge weight and assign it the correct destination
    edge->weight = weight;
    edge->destination = endNode;

    graph->noEdges++;
}

//Function used to add an the edge to both cities edge list
void graphAddEdges(struct Graph* graph, char city1[], char city2[], int weight)
{
    graphAddEdge(graph, city1, city2, weight);
    graphAddEdge(graph, city2, city1, weight);
}

//Function used to add each node and edge to the graph
void graphBuild(struct Graph* graph)
{
    //Open the file names graphEdges to be read from
    FILE * distances = fopen("distances.txt", "r");
    //Soft crash - print reason for error
    if(distances == NULL)
    {
        printf("\nDistances file not found");
    }

    int edgeWeight, doesNodeExist;
    //Char arrays to hold the city names (Again, 20 because that should comfortably hold any city name)
    char startCity[20], endCity[20];

    //Loop for each edge in the file
    while(fscanf(distances, "%s %s %d", startCity, endCity, &edgeWeight) == 3)
    {
        //Check to see if the node exists
        doesNodeExist = nodeCompare(graph, startCity);

        //Node doesn't exist so add it to the graph
        if(doesNodeExist == -1)
        {
            graphAddNode(graph, startCity, graph -> noNodes);

        }
        //Check to see if end node already exists
        doesNodeExist = nodeCompare(graph, endCity);
        //Node doesn't exist so add it
        if(doesNodeExist == -1)
        {
            graphAddNode(graph, endCity, graph -> noNodes);
        }

        //Finally add the edge to both nodes edge lists
        graphAddEdges(graph, startCity, endCity, edgeWeight);
    }
}

//TEST FUNCTION
//Function made for testing purposes, used to print the entire graph in an adjacency list-like format
void graphDisplay(struct Graph* graph)
{
    struct graphNodes* node = graph->head;
    struct graphEdges* edge;
    printf("Graph adjacency list : \n");

    //Loop to run until there are no more nodes left to print
    //print the nodeName and its edges
    while(node!=NULL)
    {
        printf("%s  ->  [", node->nodeName);
        edge = node->head;
        //Loop to print all of a nodes edges
        while(edge != NULL)
        {
            printf("(%s | %d) ", edge->destination->nodeName, edge->weight);
            edge = edge->next;
        }
        node = node->next;
        printf("] \n");
    }
}

//Function to swap the position of two nodes in the linked list
void nodeSwap(struct graphNodes* nodeA, struct graphNodes* nodeB)
{
    struct graphNodes* temp;

    temp = nodeA->next;

    //Ensure that the node's next and prev elements are set to the correct element after the swap
    nodeA->next = nodeB->next;
    nodeB->next = temp;
    nodeA->prev->next = nodeB;
    nodeB->prev->next = nodeA;

    temp = nodeA->prev;

    nodeA->prev = nodeB->prev;
    nodeB->prev = temp;
    nodeA->next->prev = nodeA;
    nodeB->next->prev = nodeB;
}

//Array to hold the route taken in the Dijkstra's algorithm
struct dijkstraRoute
{
    //Variable to hold the total distance travelled in a route
    int distanceTravelled;
    //Set the route size to be 10 as no route should have over 10 nodes in its traversal
    int routeSize;
    //Initialise the struct to be an array of graphNodes elements
    struct graphNodes** dijkstraRouteArray;
};

//Constructor for a route found using the algorithm
struct dijkstraRoute* dijkstraRouteConstructor()
{
    struct dijkstraRoute* route;

    //Allocate memory for the route array and the graphNodes elements within
    route = malloc(sizeof(struct dijkstraRoute));
    //10 because there are no roots in the graph which visit over 10 vertices
    //May have to increase this value if more nodes are added resulting in longer routes
    route->dijkstraRouteArray = malloc(10 * sizeof(struct graphNodes));

    //Initialise the field values
    route->distanceTravelled = 0;
    route->routeSize = 0;

    return route;
};

//Structure for a heap which is being stored as an array of individual heap elements
//Array because it is easier to manipulate the elements than if it was a linked list
struct Heap
{
    //Number of nodes in heap
    int noNodes;
    //Max size of heap
    int maxNodes;
    //** makes an array of element structures within the heap structure
    struct heapElements** elements;
};

//Structure for the individual elements of the heap, which have a distance from start and a node structure
struct heapElements
{
    //Distance from start node to said node
    int distanceFromStart;
    //The heap elements structure holds graphNodes elements
    struct graphNodes* node;
};

struct Heap* heapConstructor(struct Graph* graph, int heapSize)
{
    //Create the heap structure and allocate memory to the entire heap and the individual elements
    struct Heap* heap;
    heap = malloc(sizeof(struct Heap));
    heap->elements = malloc(heapSize * sizeof(struct heapElements));

    //Set the heap fields
    heap->noNodes = 0;
    heap->maxNodes = heapSize;

    return heap;
};

//Constructor for elements of the heap, contains the node structure for more versatility
struct heapElements* elementConstructor(int distanceFromStart, struct graphNodes* node)
{
    //Create the structure and allocate its memory
    struct heapElements* elements;
    elements = malloc(sizeof(struct heapElements));

    elements->distanceFromStart = distanceFromStart;

     //Set the elements node field to be the node associated with it
    elements->node = node;

    return elements;
};

//Function which swaps two elements within a heap
void heapSwap(struct Heap* heap, int elementA, int elementB)
{
    //Create a temporary element to hold one element while it is altered
    struct heapElements* tempElement = heap->elements[elementA];

    heap->elements[elementA] = heap->elements[elementB];
    heap->elements[elementB] = tempElement;
}

//Function which checks if a node has a left child and returns its index
int leftChild(struct Heap* heap, int rootIndex)
{
    int leftChildIndex = (rootIndex*2) + 1;
    //If the index of the left child is greater than the size of the heap, there is no left child
    if(leftChildIndex > heap->noNodes - 1)
    {
        return -1;
    }
    else
    {
        return leftChildIndex;
    }
}

//Same as above function but for right child
int rightChild(struct Heap* heap, int rootIndex)
{
    int rightChildIndex = (rootIndex * 2) + 2;
    if(rightChildIndex > heap->noNodes - 1)
    {
        return -1;
    }
    else
    {
        return rightChildIndex;
    }
}

//Function which checks if an element has a parent node
int parent(int elementIndex)
{
    //Check if the element is the root, if so then it has no parents
    if(elementIndex == 0)
    {
        //-1 indicates no parent node present, hence root node
        return -1;
    }
    else
    {
        //return index of parent (if we are checking if element 3 has a parent, (3-1)/2 = 1 which is its parent index)
        return (elementIndex - 1)/2;
    }
}

//Heapify function ensuring the smallest element is always at the top of the heap after an item is enqueue'd to the bottom
void heapifyUp(struct Heap* heap)
{
    //set index to be the index of the last element
    int index = heap->noNodes - 1;
    //Loop to run while the parent is larger than the children
    while(parent(index) != -1 && heap->elements[parent(index)]->distanceFromStart > heap->elements[index]->distanceFromStart)
    {
        //Swap the parent with its smaller child node
        heapSwap(heap, index, parent(index));
        index = parent(index);
    }
}

//Heapify function from a given node rather than the bottom, saving time
void heapifyUpFromIndex(struct Heap* heap, int elementIndex)
{
    //set index to be the index of the given element
    int index = elementIndex;
    //Loop to run while the parent is larger than the children
    while(parent(index) != -1 && heap->elements[parent(index)]->distanceFromStart > heap->elements[index]->distanceFromStart)
    {
        //Swap parent with smaller child nodes
        heapSwap(heap, index, parent(index));
        index = parent(index);
    }
}

//Heapify function ensuring the smallest element is on top after an item has been dequeue'd from the top
void heapifyDown(struct Heap* heap)
{
    int index = 0;
    //Loop to run while there is a left child
    while(leftChild(heap, index) != -1)
    {
        //Set variable to be the index of the smallest child node, don't know which is smallest yet so must check
        int smallestChildIndex = leftChild(heap, index);
        //Check if right child is smaller than left child, if so set smallest child index to be right
        if(rightChild(heap, index) != -1 && heap->elements[rightChild(heap, index)]->distanceFromStart < heap->elements[leftChild(heap, index)]->distanceFromStart)
        {
            smallestChildIndex = rightChild(heap, index);

        }
        //Check if the smallest element is at the root position in the heap
        if(heap->elements[index]->distanceFromStart < heap->elements[smallestChildIndex]->distanceFromStart)
        {
            return;
        }
        //If not, heap is still in incorrect order
        else
        {
            heapSwap(heap, index, smallestChildIndex);
        }
        index = smallestChildIndex;
    }
}

//TEST FUNCTION
//Function used to print the elements in a heap, used for test purposes only
void heapPrint(struct Heap* heap)
{
    printf("\nHeap Contents: ");
    int i = 0;
    for (i = 0; i < heap->noNodes; i++)
    {
        printf("\n%s  %d", heap->elements[i]->node->nodeName, heap->elements[i]->distanceFromStart);
    }

}

//Function to add an element to a heap
void heapEnqueue(struct Heap* heap, struct graphNodes* node, int distanceFromStart)
{
    struct heapElements* element = elementConstructor(distanceFromStart, node);

    //Check to see if there is room in the heap for another element TEST ONLY
    if(heap->noNodes == heap->maxNodes)
    {
        printf("\nHeap is full, cannot add element");
        return;
    }
    else
    {
        //Add element into the last location of heap
        heap->elements[heap->noNodes] = element;
        heap->noNodes++;
        heapifyUp(heap);
    }

}

//Function to remove the minimum value from the heap and return the node associated with it
struct graphNodes* heapDequeue(struct Heap* heap)
{
    //Create a temporary element to hold the element being removed
    struct heapElements* x = heap->elements[0];

    if (heap->noNodes == 0)
    {
        printf("Heap is empty, cannot dequeue");
        return NULL;
    }

    //Set the first element in the heap to be the last element
    heap->elements[0] = heap->elements[heap->noNodes - 1];
    heap->noNodes--;

    //Heapify down the tree to ensure minimum value is at index 0
    heapifyDown(heap);

    return x->node;
};

//Function which checks if a route is already in the heap
int heapCheck(struct Heap* heap, char nodeName[])
{
    int i, x;
    //Check the name of the element against every element in the heap to see if it already exists
    for (i = 0; i < heap->noNodes; i++)
    {
        x = strcmp(nodeName, heap->elements[i]->node->nodeName);
        //Already in heap
        if (x == 0)
        {
            return i;
        }
        //Not in heap
    }
    return -1;
}

//Function used to change the distance to a node in the heap if a shorter route is discovered
void heapChangeDistance(struct Heap* heap, int index, int newDistance)
{
    //Change the current distance in the heap to be the new one
    heap->elements[index]->distanceFromStart = newDistance;
    //Re-sort from the altered element, avoid sorting the entire thing
    heapifyUpFromIndex(heap, index);
}

//Function to print the output to a text file
void printDijkstraRoute(struct dijkstraRoute* dijRoute, int routeDistance, FILE* outputFile)
{
    //Print the route to be calculated to the file
    fprintf(outputFile, "The route from %s to %s: \n", dijRoute->dijkstraRouteArray[dijRoute->routeSize - 1]->nodeName, dijRoute->dijkstraRouteArray[0]->nodeName);
    int i;
    //Loop to print the nodes visited in the correct order to the file
    for(i = dijRoute->routeSize - 1; i >= 0; i--)
    {
        fprintf(outputFile, "%s     ", dijRoute->dijkstraRouteArray[i]->nodeName);
    }
    //Also print the distance travelled
    fprintf(outputFile, "\nDistance travelled = %d\n\n", routeDistance);
}

//Function to perform Dijkstra's algorithm for a given start and end node
void graphDijkstra(struct Graph* graph, int startNodeIndex, int endNodeIndex, FILE* outputFile)
{
    //Create a heap structure to hold the distances from the start node
    struct Heap* heapx = heapConstructor(graph ,graph->noNodes);

    //Get the start and end node structures
    struct graphNodes* startNode = nodeSearch(graph, startNodeIndex);
    struct graphNodes* endNode = nodeSearch(graph, endNodeIndex);

    //Create new structures to hold the current node and its edges
    //Making it easier to add its neighbours to the heap
    struct graphNodes* currentNode = startNode;
    struct graphEdges* currentNodeEdge;

    //List to hold the distance from the start node to each of the cities
    //The indexing of this list is the same as the node list
    int distanceFromStart[graph -> noNodes];

    //Array of graphNodes to hold the previously visited node, used to get the final route
    struct graphNodes** previousNodeList[graph -> noNodes];

    //List to hold the nodes(index) which have been visited
    int visitedNodesIndices[graph->noNodes];

    //Setting the current node to be the start node
    int currentNodeIndex = startNodeIndex;

    //Loop to initialise distance to all nodes from start to be infinity
    int i;
    for(i = 0; i < graph -> noNodes; i++)
    {
        distanceFromStart[i] = INFINITY;
        heapx->elements[i] = NULL;
        visitedNodesIndices[i] = -1; //not visited == -1
        previousNodeList[i] = NULL;
    }

    //Initialising the start node to have a distance from the start of 0
    distanceFromStart[startNodeIndex] = 0;

    //Setting the start node to be visited (visited == 1)
    visitedNodesIndices[startNodeIndex] = 1;

    //Loop to hold the algorithm process, not ending until we are at the end vertex
    while(currentNodeIndex != endNodeIndex)
    {
        //Set the current node to visited
        visitedNodesIndices[currentNode->index] = 1;

        currentNodeEdge = currentNode->head;

        //Loop to add the edges to be considered to the heap and update the distanceFromStart matrix
        while(currentNodeEdge != NULL)
        {
            //Check to see if the node being analysed is unvisited
            if (visitedNodesIndices[currentNodeEdge->destination->index] == -1)
            {
                //Check if the new route is shorter than the current distance from the node to the start
                if (currentNodeEdge->weight + distanceFromStart[currentNodeIndex] < distanceFromStart[currentNodeEdge->destination->index])
                {
                    //Change its distance from the start to be the new shorter route
                    distanceFromStart[currentNodeEdge->destination->index] = currentNodeEdge->weight + distanceFromStart[currentNodeIndex];
                    //Append the name of the previously visited node to the list
                    previousNodeList[currentNodeEdge->destination->index] = currentNode;
                    //Check if the element is already in the heap, if so don't add another
                    int hCheck = heapCheck(heapx, currentNodeEdge->destination->nodeName);

                    if (hCheck == -1)
                    {
                        heapEnqueue(heapx, currentNodeEdge->destination, distanceFromStart[currentNodeEdge->destination->index]);
                    }
                    else
                    {
                        heapChangeDistance(heapx, hCheck, currentNodeEdge->weight + distanceFromStart[currentNodeIndex]);
                    }
                }
            }
            currentNodeEdge = currentNodeEdge->next;
        }
        //Change the current node to be the node with least distance from the start
        currentNode = heapDequeue(heapx);
        //visitedNodesIndices[currentNode->index] == 1;
        currentNodeIndex = currentNode->index;
    }

    //Get the route
    //Construct a dijkstra Route structure to hold the nodes visited in the route
    struct dijkstraRoute* dijRoute = dijkstraRouteConstructor();

    //Set the currentNode to be the end node so we can backtrack through the visited nodes
    currentNode = endNode;

    //Create a while loop to append each node visited in the route to the structure
    i = 0;
    while(currentNode != startNode)
    {
        //Set the first element in the route array to be the end node and increment routeSize
        dijRoute->dijkstraRouteArray[i] = currentNode;
        dijRoute->routeSize++;
        //Set current node to be the node visited prior to the currentNode
        currentNode = previousNodeList[nodeCompare(graph, currentNode->nodeName)];
        i++;
    }
    //Append the start node to the array, couldn't do inside the loop because currentNode == startNode
    dijRoute->dijkstraRouteArray[i] = currentNode;
    dijRoute->routeSize++;

    printDijkstraRoute(dijRoute, distanceFromStart[endNode->index], outputFile);
}

//Function to run Dijkstra's algorithm from a file multiple times; writing the output to a text file (output.txt)
void dijkstraRouteFromFile(struct Graph* graph)
{
    //Open the files to be read and wrote to
    FILE * citypairs = fopen("citypairs.txt", "r");
    FILE * output = fopen("output.txt", "w");
    //Soft crash print statements
    if(citypairs == NULL)
    {
        printf("\nCitypairs file not found");
    }
    if(output == NULL)
    {
        printf("\nOutput file not found");
    }
    //Initialise the structures for the start and end nodes to be extracted from the files
    struct graphNodes* startNode;
    struct graphNodes* endNode;
    //Temporary char arrays to store scanned node names for use in search functions
    char nodeA[20], nodeB[20];

    //Loop which runs until there are no more lines with two strings present
    //Meaning that the loop will end after each pair of citypairs is analysed
    while(fscanf(citypairs, "%s %s", nodeA, nodeB) == 2)
    {
        startNode = nodeSearch(graph, nodeCompare(graph, nodeA));
        endNode = nodeSearch(graph, nodeCompare(graph, nodeB));
        graphDijkstra(graph, startNode->index, endNode->index, output);
    }
}
