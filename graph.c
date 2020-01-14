//
// Created by n-0 on 1/8/20.
//

#include "graph.h"
#include <stdlib.h>
#include <string.h>

/**
 * TODO (DIFFICULT)
 * make it possible to
 * expand number of attributes
 * of a vertex and keep already
 * registered values
 */

/**
 * TODO (MEDIUM)
 * create an edge
 */

uint64_t MAX_ID;

PROPERTY_T find_property_type(char *property_type) {
    if (strncmp(property_type, "INT", sizeof("INT")) == 0) {
        return PROP_INT_T;
    } else {
        return PROP_UNDEFINED;
    }
}

FLEXIBLE_T* create_value_by_type(PROPERTY_T p_type, void *p_value) {
    FLEXIBLE_T *value = (FLEXIBLE_T*) malloc(sizeof(FLEXIBLE_T));
    switch (p_type) {
        case PROP_INT_T:
            value->i = (int*) p_value;
            return value;
        case PROP_CHAR_T:
            value->c = (char*) p_value;
            return value;
        case PROP_STRING_T:
            value->s = (char*) p_value;
            return value;
        default:
            value->ud = p_value;
            return value;
    }
}

/**
 * create_id creates a unique
 * id in the current graph db.
 * @return int
 */
uint64_t create_id(ERROR_CODE *error) {
    uint64_t new_max_id = MAX_ID + 1;

    if (new_max_id < MAX_ID) goto max_id_exceeded;

    return MAX_ID++;

    max_id_exceeded:
    *error = NO_ID_LEFT;
    return 0;
}

/**
 * create_edge takes two vertices and creates
 * an edge for connecting both with an id.
 * @param start_vertex
 * @param end_vertex
 * @param error
 * @return edge*
 */

edge *create_edge(vertex *start_vertex, vertex *end_vertex, bool directed, ERROR_CODE *error) {
    edge *e = (edge*) malloc(sizeof(edge));
    uint64_t id = create_id(error);
    if (error != NO_ERROR) return e;
    e->id = id;
    e->start = start_vertex->id;
    e->end = end_vertex->id;
    e->directed = directed;
    return e;
}

/**
 * add_property_edge adds an property to
 * an edge as long as there isn't already
 * one defined.
 * @param e
 * @param property_name
 * @param property_type
 * @param property_value
 * @return
 */
void add_property_edge(edge *e, char* property_name, char* property_type, void* property_value, ERROR_CODE *error) {
    if (e->property_name != NULL) return;
    e->property_name = property_name;
    PROPERTY_T e_type  = find_property_type(property_type);
    FLEXIBLE_T *e_value = create_value_by_type(e_type, property_value);
    e->property_type = e_type;
    e->property_value = e_value;
}

/**
 * create_vertex creates a vertex
 * and allocates some memory for
 * attributes.
 * @param label char*
 * @return vertex*
 */
vertex *create_vertex(char *label, ERROR_CODE *error) {
    vertex *v = (vertex *) malloc(sizeof(vertex));

    v->id = create_id(error);
    if (*error != NO_ERROR) goto error_no_id;

    v->property_size = 0;
    v->label = label;
    v->property_names = (char **) malloc(20 * MAX_STRING * sizeof(char *));
    v->property_types = (PROPERTY_T *) malloc(20 * sizeof(PROPERTY_T));
    v->property_values = (FLEXIBLE_T *) malloc(20 * sizeof(FLEXIBLE_T));
    return v;

    error_no_id:
    return v;
}

/**
 * vertex_add_property adds a
 * property to a vertex.
 * @param n vertex*
 * @param property_name char*
 * @param property_type char*
 * @param property_value void*
 * @return vertex*
 */
void vertex_add_property(vertex *v, char *property_name, char *property_type, void *property_value) {
    PROPERTY_T type;
    FLEXIBLE_T value;
    /**
     * TODO (EASY)
     * Expand if else with all other
     * types.
     */
    if (strncmp(property_type, "INT", sizeof("INT")) == 0) {
        type = PROP_INT_T;
        (value.i) = (int *) property_value;
    } else if (strncmp(property_type, "STRING", sizeof("STRING")) == 0) {
        type = PROP_STRING_T;
        (value.s) = (char *) property_value;
    } else if (strncmp(property_type, "FLOAT", sizeof("FLOAT")) == 0) {
        type = PROP_FLOAT_T;
        (value.f) = (float *) property_value;
    }
    v->property_names[v->property_size] = property_name;
    v->property_types[v->property_size] = type;
    v->property_values[v->property_size] = value;
    v->property_size++;
}

void allocate_edge_for_vertex(vertex *v, edge *e, ERROR_CODE *error) {
    if (v->edges_size < e->id) {
        edge *new_start = realloc(v->edges, e->id);
        if (new_start == NULL) {
            *error = OUT_OF_MEMORY;
            return;
        }
        v->edges = new_start;
    }
    if (v->edges_size < e->id) {
        edge *new_start = realloc(v->edges, e->id);
        if (new_start == NULL) {
            *error = OUT_OF_MEMORY;
            return;
        }
        v->edges = new_start;
    }
    v->edges[e->id] = *e;
    v->edges_size++;
}

void allocate_edge_for_graph(graph *g, edge *e, ERROR_CODE *error);

/**
 * add_edge_to_vertex adds
 * a connection (edge) to a
 * node to its list of edges.
 * @param v
 * @param e
 */
void add_edge_to_vertex(graph *g, vertex *v1, vertex *v2, edge *e, ERROR_CODE *error) {
    if (v1->edges_size < e->id) {
        edge *new_start = realloc(v1->edges, e->id);
        if (new_start == NULL) {
            *error = OUT_OF_MEMORY;
            return;
        }
        v1->edges = new_start;
    }
    if (v1->edges_size < e->id) {
        edge *new_start = realloc(v1->edges, e->id);
        if (new_start == NULL) {
            *error = OUT_OF_MEMORY;
            return;
        }
        v1->edges = new_start;
    }
    v1->edges[e->id] = *e;
    vertex *new_start = realloc(g->vertices[v1->id], v1->edges_size)
    if (new_start == NULL) {
        *error = OUT_OF_MEMORY;
        return;
    }
    g->vertices[v->id] = new_start;
    v->edges_size++;
    if (!e->directed) {
        g->vertices[v->id] =
    } else {
        if (e->start == v->id) {

        } else {

        }
    }
    g->vertices[v->id][]
}


/**
 * create_graph creates a graph for
 * storing a database based on vertexs
 * and edges.
 * @param graph_name
 * @return
 */
graph *create_graph(char *graph_name, char *graph_path, ERROR_CODE *error) {
    graph *g = (graph*) malloc(sizeof(graph));
    char g_name[MAX_STRING];
    if (strlen(graph_name) > MAX_STRING) {
        *error = STRING_TOO_BIG;
        return g;
    }
    strncmp(g_name, graph_name, MAX_STRING - 1);
    uint64_t id = create_id(error);
    if (*error == NO_ERROR) return g;
    g->id = id;
    g->name = g_name;
    g->size_vertices = 0;
    return g;
}

void add_vertex_to_graph(graph *g, vertex *v, ERROR_CODE *error) {
    if (v->id > g->size_vertices) {
        vertex **new_start = realloc(g->vertices, v->id);
        if (new_start == NULL) {
            *error = OUT_OF_MEMORY;
            return;
        }
        g->vertices = new_start;
        g->size_vertices = v->id;
    }
}

//void add_edge_to_graph(graph *g, edge)

//vertex *find_vertex_width;
