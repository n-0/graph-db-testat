//
// Created by n-0 on 1/8/20.
//

#ifndef GRAPH_DB_TESTAT_GRAPH_H
#define GRAPH_DB_TESTAT_GRAPH_H

#include <stdint-gcc.h>
#include <stdbool.h>

#define MAX_STRING 222
#define MAX_PROPERTIES 20
#define MAX_NODES 10000
#define MAX_EDGES 40000

typedef char key_value[2][MAX_STRING];

typedef enum {
    NO_ERROR,
    STRING_TOO_BIG,
    INCORRECT_TYPE,
    NO_ID_LEFT,
} ERROR_CODE;

/**
 * PROPERTY_T is a type
 * for a flexible property
 * (node, edges).
 */
typedef enum {
    PROP_INT_T,
    PROP_INT_A_T,
    PROP_FLOAT_T,
    PROP_FLOAT_A_T,
    PROP_STRING_T,
    PROP_CHAR_T,
    PROP_DOUBLE_T,
    PROP_DOUBLE_A_T,
    PROP_BOOL_T,
    PROP_BOO_A_T
} PROPERTY_T;

/**
 * FLEXIBLE_T is used
 * to store data of multiple
 * types in one property
 * of struct like node, edge.
 */
typedef union {
    int *i;
    int *ia;
    float *f;
    float *fa;
    char *s;
    char *c;
    double *d;
    double *da;
    bool *b;
    bool *ba;
} FLEXIBLE_T;

/**
 * node has an id
 * a optional label for
 * grouping nodes.
 * The property_* attributes
 * allow setting name value pairs
 * with associated types.
 */
typedef struct {
    uint64_t id;
    char *label;
    char **property_names;
    PROPERTY_T *property_types;
    FLEXIBLE_T *property_values;
    int property_size;
} node;

/**
 * edge has an own id
 * and uses the id of nodes
 * for start/end points
 * and can take an optional
 * property name, value pair
 * with an associated type.
 */
typedef struct {
    uint64_t id;
    char start[MAX_STRING];
    char end[MAX_STRING];
    char *property_name;
    PROPERTY_T property_type;
    FLEXIBLE_T property_value;
} edge;

/**
 * graph is a wrapper
 * for presenting one
 * graph database with
 * a name, path to the
 * file where the db
 * is stored and an id
 * for interaction.
 * Nodes are accessed
 * by edges.
 */
typedef struct {
    char name[MAX_STRING];
    char path[MAX_STRING];
    uint64_t max_id;
    edge edges[];
} graph;

uint64_t create_id();

node *create_node(char *label, ERROR_CODE *error);

node *node_add_property(node *n, char *property_name, char *property_type, void *property_value);

edge *create_edge(char *start_id, char *end_id, char *label, void *value);

node *find_node_by_id(char *id);

node *find_node_by_label_property(char *label, char *property, void *value);

node *update_node(node *n, char *property, void *value);

#endif //GRAPH_DB_TESTAT_GRAPH_H
