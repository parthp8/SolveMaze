#include "graphutils.h" // header for functions to load and free adjacencyList
#include "queue.h" // header for queue

// A program to solve a maze that may contain cycles using BFS

int main ( int argc, char* argv[] ) {

    // First, read the query file to get the source and target nodes in the maze
    FILE* inputFile = fopen(argv[2], "r");
    if (!inputFile) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }
    int source, target;
    fscanf (inputFile, "%d\n%d", &source, &target);

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode* adjacencyList = NULL;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);

    // USE A QUEUE TO PERFORM BFS
    Queue queue = { .front=NULL, .back=NULL };

    // An array that keeps track of who is the parent node of each graph node we visit
    graphNode_t* parents = calloc( graphNodeCount, sizeof(graphNode_t) );
    for (size_t i = 0; i < graphNodeCount; i++) {
        parents[i] = -1; // -1 indicates that a nodes is not yet visited
    }

    int current = source;

    while ( current != target ) {
        // so long as we haven't found the target node yet, iterate through the adjacency list
        // add each neighbor that has not been visited yet (has no parents) to the queue of nodes to visit
        for (AdjacencyListNode* e = adjacencyList[current].next; e; e = e->next) {
            if (parents[e->graphNode] == -1) {
                parents[e->graphNode] = current;
                enqueue(&queue, e);
            }
        }

        // Visit the next node at the front of the queue of nodes to visit
        current = *(int*) dequeue(&queue);
    }

    // Now that we've found the target graph node, use the parent array to print maze solution
    // Print the sequence of edges that takes us from the source to the target node
    for (int i = 0; i < graphNodeCount; i++) {
        if (parents[i] != -1) printf("%ld %d\n", parents[i], i);
    }

    // free any queued graph nodes that we never visited because we already solved the maze
    while ( queue.front ) {
        dequeue(&queue);
    }
    free (parents);
    freeAdjList ( graphNodeCount, adjacencyList );

    return EXIT_SUCCESS;
}
