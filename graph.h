//
// Created by no on 1/8/20.
//

#ifndef GRAPH_DB_TESTAT_GRAPH_H
#define GRAPH_DB_TESTAT_GRAPH_H

#include <stdint-gcc.h>

#define MAX_STRING 222
#define MAX_PROPERTIES 20
#define MAX_NODES 10000
#define MAX_EDGES 40000

typedef char key_value[2][MAX_STRING];

/**
 * node has an id
 * a optional label for
 * grouping nodes, a
 * list of tuples for the
 * keys (name, type) and
 * a list of values
 */
typedef struct {
    uint64_t id;
    char *label;
    key_value keys[MAX_PROPERTIES];
    int size_keys;
    void *values;
} node;

/**
 * edge has an id
 * and uses the id of nodes
 * for start and end points
 * and can take an optional
 * key with an associated value
 */
typedef struct {
    uint64_t id;
    char start[MAX_STRING];
    char end[MAX_STRING];
    key_value key;
    void *value;
} edge;


typedef struct {
    char name[MAX_STRING];
    char path[MAX_STRING];
    uint64_t max_id;
    edge edges[];
} graph;

uint64_t create_id();

node *create_node(char *label);

node *node_add_property(node *n, key_value key, void *value);

edge *create_edge(char *start_id, char *end_id, char *label, void *value);

node *find_node_by_id(char *id);

node *find_node_by_label_property(char *label, char *property, void *value);

node *update_node(node *n, char *property, void *value);

#endif //GRAPH_DB_TESTAT_GRAPH_H
