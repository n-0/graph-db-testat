//
// Created by n-0 on 1/21/20.
//

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ops.h"


/**
 * find_index_properties finds
 * the index of a property for
 * its name, type and value.
 * @param properties
 * @param property_length
 * @param name
 * @return
 */
int find_index_properties(char **properties, int property_length, char *name) {
    int i = 0;
    while (strncmp(properties[i], name, sizeof(*name)) != 0) {
        if (i == property_length) {
            return -1;
        }
        i++;
    }
    return i;
}

/**
 * update_vertex_property updates
 * the value of a vertex.
 * @param v
 * @param property_name
 * @param property_value
 * @param e
 */
void update_vertex_property(vertex *v, char *property_name, void *property_value, ERROR_CODE *e) {
    int index_property = find_index_properties(v->property_names, v->property_size, property_name);
    if (index_property == -1) {
        *e = NOT_FOUND;
        return;
    }
    PROPERTY_T type = v->property_types[index_property];
    FLEXIBLE_T *value = create_value_by_type(type, property_value);
    free(v->property_values[index_property]);
    v->property_values[index_property] = value;
}

/**
 * update_edge_property updates the
 * value of an edge_property
 * @param e
 * @param property_value
 */
void update_edge_property(edge *e, void *property_value) {
    free(e->property_value);
    FLEXIBLE_T *value = create_value_by_type(e->property_type, property_value);
    e->property_value = value;
}

/**
 * delete_edge deletes the edge
 * between the two vertices of
 * the edge.
 * @param g
 * @param e
 * @param error
 */
void delete_edge(graph *g, edge *e, ERROR_CODE *error) {
    // TODO not found error
    // TODO check for direction

    // Remove vertex from each others adj list
    int start_index = find_index_vertex_adj_list(g, e->start);
    int end_index = find_index_vertex_adj_list(g, e->end);
    int end_in_start_index = find_vertex_adj_of_vertex(g, start_index, e->end, error);
    int start_in_end_index = find_vertex_adj_of_vertex(g, end_index, e->start, error);
    free(g->vertices[start_index]->list[end_in_start_index]);
    g->vertices[start_index]->list[end_in_start_index] = NULL;
    free(g->vertices[end_index]->list[start_in_end_index]);
    g->vertices[end_index]->list[start_in_end_index] = NULL;


    // Remove edge in edges list of vertices
    for (int i = 0; i < g->vertices[start_index]->v_pointer->edges_size; i++) {
        if (g->vertices[start_index]->v_pointer->edges[i]->id == e->id) {
            g->vertices[start_index]->v_pointer->edges[i] = NULL;
        }
    }
    for (int i = 0; i < g->vertices[end_index]->v_pointer->edges_size; i++) {
        if (g->vertices[end_index]->v_pointer->edges[i]->id == e->id) {
            g->vertices[end_index]->v_pointer->edges[i] = NULL;
        }
    }

    // Remove edge
    free(e->property_name);
    free(e->property_value);
    free(e);
}

/**
 * delete_vertex removes a vertex,
 * it's entries in adjacency
 * list and edges.
 * @param g
 * @param v
 * @param e
 */
void delete_vertex(graph *g, vertex *v, ERROR_CODE *e) {
    for (int j = 0; j < v->property_size; j++) {
        free(v->property_names[j]);
        free(v->property_values[j]);
    }

    for (int j = 0; j < g->size_vertices; j++) {
        if (g->vertices[j]->v_pointer->id == v->id) {
            free(g->vertices[j]->list);
            g->vertices[j] = NULL;
            break;
        }
        // VERY CRITICAL ERROR (how to handle?)
        //*e = NOT_FOUND;
    }


    // Unsure if necessary instead of delete_edge
    // double cost from delete_edge because
    // adjacency from v is cleared
    /*
    for (int j = 0; j < v->edges_size; j++) {
       delete_edge(g, v->edges[j], e);
    }
    */

    uint64_t vertices_to_free[v->edges_size];
    int size_vertices_to_free = 0;
    for (int j = 0; j < v->edges_size; j++) {
        if (v->edges[j]->start == v->id) {
            vertices_to_free[size_vertices_to_free++] = v->edges[j]->end;
        } else {
            vertices_to_free[size_vertices_to_free++] = v->edges[j]->start;
        }
    }
    for (int j = 0; j < size_vertices_to_free; j++) {

        // Remove from adjacency list
        // TODO error handling of not found
        int graph_index = find_index_vertex_adj_list(g, vertices_to_free[j]);
        int adj_list_index = find_vertex_adj_of_vertex(g, graph_index, v->id, e);
        g->vertices[graph_index]->list[adj_list_index] = NULL;


        // Remove associated edges
        for (int k = 0; k < g->vertices[graph_index]->v_pointer->edges_size; k++) {
            if (g->vertices[graph_index]->v_pointer->edges[k]->start == v->id
                || g->vertices[graph_index]->v_pointer->edges[k]->end == v->id
                    ) {
                free(g->vertices[graph_index]->v_pointer->edges[k]);
                g->vertices[graph_index]->v_pointer->edges[k] = NULL;
                break;
            }
        }
    }

//    free(v->label); breaks for some reason
    free(v->edges);
    free(v->property_types);
    free(v);
}

bool check_flexible_equal(FLEXIBLE_T *f1, FLEXIBLE_T *f2, PROPERTY_T type) {
    switch (type) {
        case PROP_INT_T:
            return *(f1->i) == *(f2->i);
        case PROP_FLOAT_T:
            return *(f1->f) == *(f2->f);
        case PROP_CHAR_T:
            return *(f1->c) == *(f2->c);
        case PROP_STRING_T:
            return strcmp(f1->s, f2->s) == 0;
        case PROP_DOUBLE_T:
            return *(f1->d) == *(f2->d);
        case PROP_BOOL_T:
            return *(f1->b) == *(f2->b);
        default:
            return false;
    }
}

vertex **search_by_property(graph *g, char *property_name, char *property_type, void *property_value, ERROR_CODE *e) {
    vertex **found = (vertex **) malloc(10 * sizeof(vertex **));
    int found_size = 0;
    int found_max_size = 10;
    PROPERTY_T type = find_property_type(property_type);
    FLEXIBLE_T *value = create_value_by_type(type, property_value);
    for (int i = 0; i < g->size_vertices; i++) {
        vertex *v = g->vertices[i]->v_pointer;
        int property_index = find_index_properties(v->property_names, v->property_size, property_name);
        if (property_index != -1) {
            if (check_flexible_equal(v->property_values[property_index], value, type)) {
                if (found_max_size < found_size + 1) {
                    vertex **new_start = realloc(found, found_max_size + 10);
                    if (new_start == NULL) {
                        *e = OUT_OF_MEMORY;
                        return found;
                    }
                }
                found[found_size++] = v;
                found_max_size++;
            }
        }
    }
    return found;
}

