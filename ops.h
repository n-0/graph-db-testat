//
// Created by n-0 on 1/21/20.
//

#ifndef GRAPH_DB_TESTAT_OPS_H
#define GRAPH_DB_TESTAT_OPS_H

#include "graph.h"

void update_vertex_property(vertex *v, char *property_name, void *property_value, ERROR_CODE *e);

void update_edge_property(edge *e, void *property_value);

void delete_vertex(graph *g, vertex *v, ERROR_CODE *e);

void delete_edge(graph *g, edge *e, ERROR_CODE *error);

vertex **search_by_property(graph *g, char *property_name, char *property_type, void *property_value, ERROR_CODE *e);

#endif //GRAPH_DB_TESTAT_OPS_H
