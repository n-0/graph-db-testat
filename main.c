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

int main() {
    ERROR_CODE error = NO_ERROR;
    node *n = create_node("example", &error);
    if (error != NO_ERROR) {
        printf("Unable to create node\n");
        exit(1);
    }
    char *name = "age";
    char *type = "INT";
    int value = 19;
    node_add_property(n, name, type, &value);
    printf("Node label: %s\nNode value: %d\n", n->label, *n->property_values[0].i);
    return 0;
}
