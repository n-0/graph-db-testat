//
// Created by no on 1/8/20.
//

#include "graph.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

uint64_t MAX_ID;


void test_of_arrays() {
    int a[2][2];
    int b[2] = {1, 2};
    memcpy(a[0], b, sizeof(b));
}

uint64_t create_id() {
    return MAX_ID++;
}

node *create_node(char *label) {
    node *n = (node *) malloc(sizeof(node));
    n->id = create_id();
    n->size_keys = 0;
    n->label = label;
    return n;
}

node *node_add_property(node *n, key_value k, void *value) {
    memcpy(n->keys[n->size_keys + 1], k, sizeof(*k));
    memcpy(n->values[n->size_keys + 1], value, sizeof(value));
    n->size_keys++;
    return n;
}

