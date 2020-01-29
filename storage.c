//
// Created by no on 1/28/20.
//

#include "hashmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libxml2/libxml/encoding.h>
#include <libxml2/libxml/xmlwriter.h>
#include "graph.h"
#include "storage.h"
#include "ops.h"

#define MY_ENCODING "UTF-16"


/**
 * cast_value_string returns
 * the string for the value
 * stored in a FLEXIBLE_T
 * @param p_type
 * @param flex
 * @return
 */
char *cast_value_string(PROPERTY_T p_type, FLEXIBLE_T *flex) {
    char *value = (char *) malloc(666 * sizeof(char));
    switch (p_type) {
        case PROP_INT_T:
            sprintf(value, "%d", *flex->i);
            return value;
        case PROP_FLOAT_T:
            sprintf(value, "%f", *flex->f);
            return value;
        case PROP_CHAR_T:
            sprintf(value, "%c", *flex->c);
            return value;
        case PROP_STRING_T:
            sprintf(value, "%s", flex->s);
            return value;
        case PROP_DOUBLE_T:
            sprintf(value, "%f", *flex->d);
            return value;
        case PROP_BOOL_T:
            sprintf(value, "%d", *flex->b);
            return value;
        default:
            sprintf(value, "%s", flex->ud);
            return value;
    }
}

/**
 * cast_type_string returns
 * the string representation
 * for given type.
 * @param p_type
 * @return
 */
char *cast_type_string(PROPERTY_T p_type) {
    char *type = (char *) malloc(15 * sizeof(char));
    switch (p_type) {
        case PROP_INT_T:
            type = "INT";
            return type;
        case PROP_FLOAT_T:
            type = "FLOAT";
            return type;
        case PROP_CHAR_T:
            type = "CHAR";
            return type;
        case PROP_STRING_T:
            type = "STRING";
            return type;
        case PROP_DOUBLE_T:
            type = "DOUBLE";
            return type;
        case PROP_BOOL_T:
            type = "BOOL";
            return type;
        default:
            type = "UNDEFINED";
            return type;
    }
}

/**
 * save_db stores a graph
 * at the path of g in
 * the graphml format.
 * @param g
 * @param e
 */
void save_db(graph *g, ERROR_CODE *e) {
    int rc;
    xmlTextWriterPtr writer;
    char *uri = (char *) malloc(strlen(g->path) * sizeof(char));
    strncpy(uri, g->path, strlen(g->path));
    writer = xmlNewTextWriterFilename(uri, 0);
    free(uri);
    if (writer == NULL) {
        *e = NOT_FOUND;
        return;
    }
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        *e = NOT_FOUND;
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "graphml");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi",
                                     BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xsi:schemaLocation",
                                     BAD_CAST "http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd>");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://graphml.graphdrawing.org/xmlns");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "graph");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    char *name = (char *) malloc(strlen(g->name) * sizeof(char));
    strncpy(name, g->path, strlen(g->path));
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST name);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    map_t stored_edges = hashmap_new();
    bool was_there = true;
    for (int i = 0; i < g->size_vertices; i++) {
        vertex *v = g->vertices[i]->v_pointer;
        rc = xmlTextWriterStartElement(writer, BAD_CAST "node");
        if (rc < 0) {
            *e = WRITE_ERROR;
            return;
        }
        rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST id_to_string(v->id));
        if (rc < 0) {
            *e = WRITE_ERROR;
            return;
        }
        char *attr = (char *) malloc(666 * sizeof(char));
        for (int p_index = 0; p_index < v->property_size; p_index++) {
            char *type_string = cast_type_string(v->property_types[p_index]);
            char *value_string = cast_value_string(v->property_types[p_index], v->property_values[p_index]);
            sprintf(attr, "%s %s", v->property_names[p_index], type_string);
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST attr, BAD_CAST value_string);
            free(type_string);
            free(value_string);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
        }
        rc = xmlTextWriterEndElement(writer);
        if (rc < 0) {
            *e = WRITE_ERROR;
            return;
        }
        for (int e_index = 0; e_index < v->edges_size; e_index++) {
            char *edge_id = id_to_string(v->edges[e_index]->id);
            if (vertex_in_hashmap(stored_edges, edge_id)) continue;
            hashmap_put(stored_edges, edge_id, &was_there);
            rc = xmlTextWriterStartElement(writer, BAD_CAST "edge");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST edge_id);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "source",
                                             BAD_CAST id_to_string(v->edges[e_index]->start));
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "target", BAD_CAST id_to_string(v->edges[e_index]->end));
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            free(edge_id);
            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
        }
        free(attr);
    }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    xmlFreeTextWriter(writer);
}
