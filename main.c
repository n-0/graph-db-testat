#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

/**
 * TODO (DIFFICULT)
 * Create a test environment
 * with Google Test.
 */

/**
 * TODO (MEDIUM)
 * (After test env)
 * Test all functionality.
 */

 void test_vertex_and_properties(){ //erstellt ein Vertex und fügt ihm Eigenschaften hinzu (wie z.B. Name, Alter etc.)
   ERROR_CODE error = NO_ERROR;
   vertex *v = create_vertex("Jorgos", &error);
   if (error != NO_ERROR) {
     printf("Unable to create node\n");
     return;
   }
   char *name = "nationality";
   char *type = "STRING";
    char *value = "German";
    vertex_add_property(v, name, type, (void *) &value);
    printf("Node label: %s\nNode value: %s\n", v->label, *(v->property_values[0][0]).s);
 }

void test_graph() {
    ERROR_CODE error = NO_ERROR;
    char *graph_name = "test_graph";
    char *graph_path = "/example/path";
    graph *g = create_graph(graph_name, graph_path, &error);
    if (error != NO_ERROR) {
        printf("test_graph creation: failed");
        return;
    } else {
        printf("test_graph creation: success\n");
    }
    vertex *v1 = create_vertex("person1", &error);
    vertex *v2 = create_vertex("person1", &error);
    if (error != NO_ERROR) {
        printf("test_graph add vertex: failed (indirect)");
        return;
    }
    add_vertex_to_graph(g, v1, &error);
    add_vertex_to_graph(g, v2, &error);
    if (error != NO_ERROR) {
        printf("test_graph add vertex: failed");
        return;
    } else {
        printf("test_graph add vertex: success\n");
    }
    edge *e = create_edge(v1, v2, false, &error);
    if (error != NO_ERROR) {
        printf("test_graph add edge: failed (indirect)");
        return;
    }
    add_edge_to_graph(g, v1, v2, e, &error);
    if (error != NO_ERROR) {
        printf("test_graph add edge: failed");
        return;
    } else {
        printf("test_graph add edge: success\n");
    }
}


int main() {
    test_vertex_and_properties();
    test_graph();
    return 0;
}
