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
 * of a node and keep already
 * registered values
 */

/**
 * TODO (MEDIUM)
 * create an edge
 */

uint64_t MAX_ID;

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
 * create_node creates a node
 * and allocates some memory for
 * attributes.
 * @param label char*
 * @return node*
 */
node *create_node(char *label, ERROR_CODE *error) {
    node *n = (node *) malloc(sizeof(node));

    n->id = create_id(error);
    if (*error != NO_ERROR) goto error_no_id;

    n->property_size = 0;
    n->label = label;
    n->property_names = (char **) malloc(20 * MAX_STRING * sizeof(char *));
    n->property_types = (PROPERTY_T *) malloc(20 * sizeof(PROPERTY_T));
    n->property_values = (FLEXIBLE_T *) malloc(20 * sizeof(FLEXIBLE_T));
    return n;

    error_no_id:
    return n;
}

/**
 * node_add_property adds a
 * property to a node.
 * @param n node*
 * @param property_name char*
 * @param property_type char*
 * @param property_value void*
 * @return node*
 */
node *node_add_property(node *n, char *property_name, char *property_type, void *property_value) {
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
    n->property_names[n->property_size] = property_name;
    n->property_types[n->property_size] = type;
    n->property_values[n->property_size] = value;
    n->property_size++;
    return n;
}

