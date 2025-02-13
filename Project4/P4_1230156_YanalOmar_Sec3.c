// Yanal Omar
// 1230156
// Sec. 3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>// used this library to use the max integer
#define MAX_ROUTERS 50//define the max size of the matrix, you can change it to any number you want

struct Graph { // struct for the graph
    int matrix[MAX_ROUTERS][MAX_ROUTERS]; // reserve space for the matrix (2d array)
    char routers[MAX_ROUTERS][10]; // this is an array of strings to save the routers (the router name can be up to 9 characters)
    int routerCount; // keep track of the number of routers
};

typedef struct Graph* graph;// rename (struct Graph*) to graph for easier calling

int path[MAX_ROUTERS]; // define a global array so we can use it everywhere in the code and to save up space and time
int pathLength = 0; // keep track of the length of the array

int findRouterIndex(graph g, char *router) { //function to find the index of the router. O(n)
    for (int i = 0; i < g->routerCount; i++) { // iterate over the routers to find the router we are looking for
        if (strcmp(g->routers[i], router) == 0) return i;
    }
    return -1; // return -1 if we didnt find the router
}

void addRouter(graph g, char *router) {// function to add router O(n)
    if (findRouterIndex(g, router) == -1) { // if router does not already exist, add it to the routers array
        strcpy(g->routers[g->routerCount++], router);
    }
}

void loadRouters(graph g, char *fileName) {// function to load routers from file O(n^2)
    FILE *file = fopen(fileName, "r");// open the file for reading
    if (file == NULL) {// if file is NULL, print an error message and return to the main
        printf("Error: Unable to open file %s\n", fileName);
        return;
    }

    char line[50]; // make an array of 50 characters, to save each line in the file
    char *from, *to, *latencyStr;
    int latency;
    for (int i = 0; i < MAX_ROUTERS; i++) { // initialize the values of the matrix
        for (int j = 0; j < MAX_ROUTERS; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INT_MAX; // if i equals j, save 0 ( distance from a place to itself), else assign it to infinity
        }
    }

    while (fgets(line, sizeof(line), file)) {// while the file didnt end
        from = strtok(line, "-"); // save the source router using strtoken
        to = strtok(NULL, "-"); // save the destination router using strtoken
        latencyStr = strtok(NULL, "\n"); // save the latency as a string to save it later as an int
        if (from && to && latencyStr) {// handle the case where the line has wrong format
            latency = atoi(latencyStr);// change the latency from string to integer
            addRouter(g, from); // add router if it doesnt previously exist
            addRouter(g, to); // add router if it doesnt previously exist
            int fromIndex = findRouterIndex(g, from); // find index of the source router
            int toIndex = findRouterIndex(g, to); // find index of the destination router
            g->matrix[fromIndex][toIndex] = latency; // save the latency at the matrix[source Router][Destination Router]
            g->matrix[toIndex][fromIndex] = latency; // save the latency at the matrix[Destination Router][source Router]
            // this makes the graph undirected
        } else {
            printf("Skipping invalid line");// print a message if the line is invalide
        }
    }
    fclose(file);// close the file so no leaks happen
}

void dijkstra(graph g, int src, int dest, int *dist) { // dijkstra algorithm to find the shortest path O(V^2)
    int prev[MAX_ROUTERS], visited[MAX_ROUTERS] = {0}; // initialize previous and visited arrays
    int totalCost = 0; // variable to keep track of the total cost

    for (int i = 0; i < g->routerCount; i++) { // initialize distances to infinity and previous nodes to -1
        dist[i] = INT_MAX;
        prev[i] = -1;
    }
    dist[src] = 0; // set the distance to the source node as 0

    for (int i = 0; i < g->routerCount; i++) { // iterate over all routers
        int u = -1; // variable to store the node with the smallest distance
        for (int j = 0; j < g->routerCount; j++) // find the unvisited node with the smallest distance
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;

        if (dist[u] == INT_MAX) break; // if no reachable node remains, break
        visited[u] = 1; // mark the current node as visited

        for (int v = 0; v < g->routerCount; v++) { // iterate over all neighbors of the current node
            if (g->matrix[u][v] != INT_MAX && dist[u] + g->matrix[u][v] < dist[v]) { // if a shorter path is found
                dist[v] = dist[u] + g->matrix[u][v]; // update the shortest distance to the neighbor
                prev[v] = u; // set the current node as the parent of the neighbor
            }
        }
    }

    pathLength = 0; // reset the path length to 0
    int current = dest; // start from the destination node
    while (current != -1) { // backtrack from the destination to the source
        path[pathLength++] = current; // add the current node to the path
        current = prev[current]; // move to the parent node
    }
}

// ** i used a simple queue because it cant exceed the number of max routers, so no need to implement a queue and handle cases like the rear becoming more than size
void bfs(graph g, int src, int dest) { // BFS algorithm to find the shortest path in an unweighted graph O(V+E)
    int queue[MAX_ROUTERS], front = 0, rear = 0; // define a queue with front and rear pointers
    int visited[MAX_ROUTERS] = {0}, prev[MAX_ROUTERS]; // initialize visited and previous arrays
    int totalCost = 0; // variable to store the total cost of the path

    for (int i = 0; i < g->routerCount; i++) prev[i] = -1; // set all previous nodes to -1

    queue[rear++] = src; // enqueue the source node
    visited[src] = 1; // mark the source node as visited

    while (front < rear) { // while the queue is not empty
        int u = queue[front++]; // dequeue the front node
        for (int v = 0; v < g->routerCount; v++) { // iterate over all neighbors of the current node
            if (!visited[v] && g->matrix[u][v] != INT_MAX) { // if the neighbor is not visited and there is an edge
                visited[v] = 1; // mark the neighbor as visited
                queue[rear++] = v; // enqueue the neighbor
                prev[v] = u; // set the current node as the parent of the neighbor
                if (v == dest) break; // stop if the destination node is found
            }
        }
    }
    pathLength = 0; // reset the path length to 0
    int current = dest; // start from the destination node
    while (current != -1) { // backtrack from the destination to the source
        path[pathLength++] = current; // add the current node to the path
        if (pathLength > 1) totalCost += g->matrix[current][prev[current]]; // add the cost of the edge to the total cost
        current = prev[current]; // move to the parent node
    }
}


void saveShortestPaths(graph g, int src, int dest) { // save the paths to the file O(n)
    FILE *outputFile = fopen("shortest_distance.txt", "w");// open the file for writing
    if (outputFile == NULL) {
        printf("Error: Unable to create file.\n");
        return;
    }

    fprintf(outputFile, "Source: %s\n", g->routers[src]);// print the source router
    fprintf(outputFile, "Destination: %s\n\n", g->routers[dest]);// print the destination router
    int dist[MAX_ROUTERS]; // define the dist array to send it to the dijkstra function
    dijkstra(g, src, dest, dist);
    fprintf(outputFile, "Dijkstra: ");
    for (int i = pathLength - 1; i >= 0; i--) {// print the path by iterating over the global array "path"
        fprintf(outputFile, "%s", g->routers[path[i]]);
        if (i > 0) fprintf(outputFile, " -> ");
    }
    fprintf(outputFile, " with a total cost of %d\n", dist[dest]); // print the cost of the distance
    int totalCost = 0; // initialize an integer to find the total cost
    bfs(g, src, dest);// call the bfs dunction
    fprintf(outputFile, "BFS: ");
    for (int i = pathLength - 1; i >= 0; i--) {// iterate to print the path
        fprintf(outputFile, "%s", g->routers[path[i]]);
        if (i > 0) fprintf(outputFile, " -> ");
        if (i > 0) totalCost += g->matrix[path[i]][path[i - 1]];// add the latency to the total cost
    }
    fprintf(outputFile, " with a total cost of %d\n", totalCost);// print the total cost

    fclose(outputFile);// close the file so no leakage happen
    printf("Results saved to shortest_distance.txt\n");
}



int main() {
    printf("Welcome to the Network Packet Routing System\n");
    int choice = 0;
    char srcRouter[10], destRouter[10];
    int src = -1, dest = -1;
    graph g = (graph)malloc(sizeof(struct Graph));// initialize the graph and reserve space for it.
    g->routerCount = 0;
    do {
        printf("1- Load Routers\n");
        printf("2- Enter Source\n");
        printf("3- Enter Destination\n");
        printf("4- Save Results and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            loadRouters(g, "routers.txt"); // load the graph
        } else if (choice == 2) {
            printf("Enter source router: ");
            scanf("%s", srcRouter); // enter the source router
            src = findRouterIndex(g, srcRouter); // find the index of the source router
            if (src == -1) {// if source doesnt exist
                printf("Error: Source router not found.\n");
            } else {
                printf("Source router set to: %s\n", srcRouter);
            }
        } else if (choice == 3) {
            printf("Enter destination router: ");
            scanf("%s", destRouter); // enter the destination router
            dest = findRouterIndex(g, destRouter); // find the index of the destination router
            if (dest == -1) { // if dest is -1 then it doesnt exist
                printf("Error: Destination router not found.\n");
            } else if (src == -1) { // if source is -1 then either it doesnt exist or we didnt enter the source yet
                printf("Error: Set the source router first.\n");
            } else {// else, the dest and src exist
                printf("Source and destination set successfully.\n");
                int dist[MAX_ROUTERS];
                dijkstra(g, src, dest, dist);// call the dijkstra too save the path in the path array
                // everything bellow is like the save in file
                printf("Dijkstra: ");
                for (int i = pathLength - 1; i >= 0; i--) {
                    printf("%s", g->routers[path[i]]);
                    if (i > 0) printf(" -> ");
                }
                printf(" with a total cost of %d\n", dist[dest]);
                bfs(g, src, dest);
                int totalCost = 0;
                printf("BFS: ");
                for (int i = pathLength - 1; i >= 0; i--) {
                    printf("%s", g->routers[path[i]]);
                    if (i > 0) printf(" -> ");
                    if (i > 0) totalCost += g->matrix[path[i]][path[i - 1]];
                }
                printf(" with a total cost of %d\n", totalCost);
            }
        }else if (choice == 4) {
            if (src == -1 || dest == -1) { // of source or destination is -1 then we cant calculate the path
                printf("Error: Source and destination routers must be set before saving results.\n");
            } else {
                saveShortestPaths(g, src, dest);// save to file
            }
        } else {
            printf("Wrong input, please try again.\n");
        }
    } while (choice != 4);
    printf("Thanks for using the system! Goodbye :)\n");
    free(g);// free the graph for better space complexity
}
