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

<<<<<<< HEAD
typedef enum {
    NO_ERROR,
    STRING_TOO_BIG,
    INCORRECT_TYPE,
    OUT_OF_MEMORY,
    NO_ID_LEFT,
} ERROR_CODE;

/**
 * PROPERTY_T is a type
 * for a flexible property
 * (vertex, edges).
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
    PROP_BOO_A_T,
    PROP_UNDEFINED,
} PROPERTY_T;

/**
 * FLEXIBLE_T is used
 * to store data of multiple
 * types in one property
 * of struct like vertex, edge.
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
    void *ud;
} FLEXIBLE_T;

=======
>>>>>>> parent of 27800a3... Changed structs of edge and node
/**
 * edge has an own id
 * and uses the id of vertices
 * for start/end points.
 * It can take an optional
 * property name, value pair
 * with an associated type (weight).
 * If start and end only have their
 * literal meaning if directed is true.
 */
typedef struct {
    uint64_t id;
    uint64_t start;
    uint64_t end;
    char *property_name;
    PROPERTY_T property_type;
    FLEXIBLE_T *property_value;
    bool directed;
} edge;

/**
 * vertex has an id
 * a optional label for
<<<<<<< HEAD
 * grouping vertices.
 * The property_* attributes
 * allow setting name value pairs
 * with associated types.
=======
 * grouping nodes, a
 * list of tuples for the
 * keys (name, type) and
 * a list of values
>>>>>>> parent of 27800a3... Changed structs of edge and node
 */
typedef struct {
    uint64_t id;
    char *label;
<<<<<<< HEAD
    int property_size;
    char **property_names;
    PROPERTY_T *property_types;
    FLEXIBLE_T *property_values;
    int edges_size;
    edge *edges[];
} vertex;


/**
 * graph is a wrapper
 * for a graph database
 * with a name, path to
 * the file where the db
 * is stored and an id
 * for interaction.
 * The representation is
 * an adjacency list, where
 * the id of a vertex is its
 * index in the list.
 */
=======
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


>>>>>>> parent of 27800a3... Changed structs of edge and node
typedef struct {
    uint64_t id;
    char *name;
    char *path;
    uint64_t max_id;
    int size_vertices;
    vertex **vertices;
} graph;

uint64_t create_id();

<<<<<<< HEAD
vertex *create_vertex(char *label, ERROR_CODE *error);

void vertex_add_property(vertex *n, char *property_name, char *property_type, void *property_value);
=======
node *create_node(char *label);

node *node_add_property(node *n, key_value key, void *value);
>>>>>>> parent of 27800a3... Changed structs of edge and node

edge *create_edge(vertex *start_vertex, vertex *end_node, bool vertex, ERROR_CODE *error);

vertex *find_vertex_by_id(char *id);

vertex *find_vertex_by_label_property(char *label, char *property, void *value);

vertex *update_vertex(vertex *n, char *property, void *value);

#endif //GRAPH_DB_TESTAT_GRAPH_H
