#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "ops.h"

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

void test_search_vertex() {
    ERROR_CODE error = NO_ERROR;
    char *graph_name = "test_graph";
    char *graph_path = "/example/path";
    graph *g = create_graph(graph_name, graph_path, &error);
    if (error != NO_ERROR) {
        printf("test_search_vertex find vertex: failed (indirect)");
        return;
    }
    vertex *v1 = create_vertex("person1", &error);
    vertex *v2 = create_vertex("person1", &error);
    if (error != NO_ERROR) {
        printf("test_search_vertex find vertex: failed (indirect)");
        return;
    }
    char *property_v1_name = "Name";
    char *property_v1_type = "STRING";
    char *property_v1_value = "Niki";
    char *property_v2_name = "Name";
    char *property_v2_type = "STRING";
    char *property_v2_value = "Jorgos";
    vertex_add_property(v1, property_v1_name, property_v1_type, property_v1_value);
    vertex_add_property(v2, property_v2_name, property_v2_type, property_v2_value);
    add_vertex_to_graph(g, v1, &error);
    add_vertex_to_graph(g, v2, &error);
    if (error != NO_ERROR) {
        printf("test_search_vertex find vertex: failed (indirect)");
        return;
    }
    vertex **found = search_by_property(g, property_v1_name, property_v1_type, property_v2_value, &error);
    if (error != NO_ERROR) {
        printf("test_search_vertex find vertex: failed");
    }
    if (found[0]->id == v2->id) {
        printf("test_search_vertex find vertex: success");
    } else {
        printf("test_search_vertex find vertex: failed");
    }
}

void test_delete_vertex() {
    ERROR_CODE error = NO_ERROR;
    char *graph_name = "test_graph";
    char *graph_path = "/example/path";
    graph *g = create_graph(graph_name, graph_path, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex delete vertex: failed (indirect)");
        return;
    }
    char *label1 = "person1";
    char *label2 = "person2";
    vertex *v1 = create_vertex(label1, &error);
    vertex *v2 = create_vertex(label2, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex delete vertex: failed (indirect)");
        return;
    }
    add_vertex_to_graph(g, v1, &error);
    add_vertex_to_graph(g, v2, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex delete vertex: failed (indirect)");
        return;
    }
    edge *e = create_edge(v1, v2, false, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex delete vertex: failed (indirect)");
        return;
    }
    add_edge_to_graph(g, v1, v2, e, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex delete vertex: failed (indirect)");
        return;
    }
    uint64_t id_copy = v1->id;
    delete_vertex(g, v1, &error);
    if (error != NO_ERROR) {
        printf("test_delete_vertex find vertex: failed");
    }
    if (find_index_vertex_adj_list(g, id_copy) != -1) {
        printf("test_delete_vertex find vertex: failed");
    } else {
        printf("test_delete_vertex find vertex: success");
    }
}


int main() {
    //test_vertex_and_properties();
    //test_graph();
    test_search_vertex();
    test_delete_vertex();
    return 0;
}
