//
// Created by n-0 & jor-dro on 1/8/20.
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
    if (strncasecmp(property_type, "INT", sizeof("INT")) == 0) {
        return PROP_INT_T;
    } else if (strncasecmp(property_type, "INT_A", sizeof("INT_A")) == 0) {
        return PROP_INT_A_T;
    } else if (strncasecmp(property_type, "FLOAT", sizeof("FLOAT")) == 0) {
        return PROP_FLOAT_T;
    } else if (strncasecmp(property_type, "FLOAT_A", sizeof("FLOAT_A")) == 0) {
        return PROP_FLOAT_A_T;
    } else if (strncasecmp(property_type, "STRING", sizeof("STRING")) == 0) {
        return PROP_STRING_T;
    } else if (strncasecmp(property_type, "CHAR", sizeof("CHAR")) == 0) {
        return PROP_CHAR_T;
    } else if (strncasecmp(property_type, "DOUBLE", sizeof("DOUBLE")) == 0) {
        return PROP_DOUBLE_T;
    } else if (strncasecmp(property_type, "DOUBLE_A", sizeof("DOUBLE_A")) == 0) {
        return PROP_DOUBLE_A_T;
    } else if (strncasecmp(property_type, "BOOL", sizeof("BOOL")) == 0) {
        return PROP_BOOL_T;
    } else if (strncasecmp(property_type, "BOOL_A", sizeof("BOOL_A")) == 0) {
        return PROP_BOOL_A_T;
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
        case PROP_INT_A_T:
            value->ia = (int**) p_value;
            return value;
        case PROP_FLOAT_T:
            value->f = (float*) p_value;
            return value;
        case PROP_FLOAT_A_T:
            value->fa = (float**) p_value;
            return value;
        case PROP_CHAR_T:
            value->c = (char*) p_value;
            return value;
        case PROP_STRING_T:
            value->s = (char*) p_value;
            return value;
        case PROP_DOUBLE_T:
            value->d = (double*) p_value;
            return value;
        case PROP_DOUBLE_A_T:
            value->da = (double**) p_value;
            return value;
        case PROP_BOOL_T:
            value->b = (bool*) p_value;
            return value;
        case PROP_BOOL_A_T:
            value->ba = (bool**) p_value;
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
    if (*error != NO_ERROR) return e;
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
    FLEXIBLE_T *value;
    type = find_property_type(property_type);
    value = create_value_by_type(type, property_value);
    v->property_names[v->property_size] = property_name;
    v->property_types[v->property_size] = type;
    v->property_values[v->property_size] = value;
    v->property_size++;
}

/**
 * find_index_vertex_adj_list finds
 * the index of a vertex pointer
 * in a list of vertices.
 * @param list_vertices
 * @param id
 * @return
 */
int find_index_vertex_adj_list(graph *g, uint64_t id) {
    int i = 0;
    while (true) {
        if (g->vertices[i]->v_pointer->id == id) return i;
        if (i > g->vertices[i]->list_size) return -1;
        i++;
    }
}

/**
 * find_index_edge finds
 * the index of a vertex pointer
 * in a list of vertices.
 * @param list_vertices
 * @param id
 * @return
 */
int find_index_edge(vertex **list_edge, uint64_t id) {
    int i = 0;
    while ((*(*(list_edge + i))).id != id) i++;
    return i;
}

void allocate_vertex_graph(graph *g, vertex *v, ERROR_CODE *error) {
    adjacency_list **new_start = realloc(g->vertices, g->size_vertices + 1);
    if (new_start == NULL) {
        *error = OUT_OF_MEMORY;
        return;
    }
    g->vertices = new_start;
}

/**
 * allocate_edge_for_graph allocates the memory
 * for the vertices in the adjacency list of
 * a vertex.
 * @param g
 * @param id
 * @param e
 * @param error
 */
void allocate_edge_for_graph(graph *g, vertex *v1, vertex *v2, edge *e, ERROR_CODE *error) {
    int index = find_index_vertex_adj_list(g, v1->id);
    if (index == -1) {
        *error = NOT_FOUND;
        return;
    }
    vertex **new_start = realloc(g->vertices[index]->list, g->vertices[index]->list_size + 1);
    if (new_start == NULL) {
        *error = OUT_OF_MEMORY;
        return;
    }
    g->vertices[index]->list = new_start;
    g->vertices[index]->list[g->vertices[index]->list_size] = v2;
    g->vertices[index]->list_size++;
};

void allocate_edge_vertex(vertex *v, edge *e, ERROR_CODE *error) {
    edge **new_start = realloc(v->edges, v->edges_size + 1);
    if (new_start == NULL) {
        *error = OUT_OF_MEMORY;
        return;
    }
    v->edges = new_start;
    v->edges[v->edges_size] = e;
    v->edges_size++;
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
    allocate_vertex_graph(g, v, error);
    if (*error != NO_ERROR) return;
    adjacency_list *adj = (adjacency_list *) malloc(sizeof(adjacency_list));
    vertex **v_list = (vertex **) malloc(sizeof(vertex **));
    adj->vertices_size = 0;
    adj->list = v_list;
    adj->list_size = 0;
    adj->v_pointer = v;
    g->vertices[g->size_vertices] = adj;
    g->size_vertices++;
}

void add_edge_to_graph(graph *g, vertex *v1, vertex *v2, edge *e, ERROR_CODE *error) {
    allocate_edge_vertex(v1, e, error);
    if (*error != NO_ERROR) {
        *error = OUT_OF_MEMORY;
        return;
    }
    allocate_edge_vertex(v2, e, error);
    if (*error != NO_ERROR) {
        *error = OUT_OF_MEMORY;
        return;
    }
    if (!e->directed) {
        allocate_edge_for_graph(g, v1, v2, e, error);
        if (*error != NO_ERROR) {
            *error = OUT_OF_MEMORY;
            return;
        }
        allocate_edge_for_graph(g, v2, v1, e, error);
        if (*error != NO_ERROR) {
            *error = OUT_OF_MEMORY;
            return;
        }
    } else {
        if (v1->id == e->end) {
            allocate_edge_for_graph(g, v1, v2, e, error);
            if (*error != NO_ERROR) {
                *error = OUT_OF_MEMORY;
                return;
            }
        } else {
            allocate_edge_for_graph(g, v2, v1, e, error);
            if (*error != NO_ERROR) {
                *error = OUT_OF_MEMORY;
                return;
            }
        }
    }
}
