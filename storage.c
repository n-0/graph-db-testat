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
    bool was_there = true;
    int rc;
    xmlTextWriterPtr writer;
    char *uri = (char *) malloc((strlen(g->path) + 1) * sizeof(char));
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
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://graphml.graphdrawing.org/xmlns");
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
                                     BAD_CAST "http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:y", BAD_CAST "http://www.yworks.com/xml/graphml");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterStartElement(writer, BAD_CAST "key");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "for", BAD_CAST "node");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST "d3");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "yfiles.type", BAD_CAST "nodegraphics");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    map_t added_props = hashmap_new();
    for (int i = 0; i < g->size_vertices; i++) {
        vertex *v = g->vertices[i]->v_pointer;
        for (int p = 0; p < v->property_size; p++) {
            if (!vertex_in_hashmap(added_props, v->property_names[p])) {
                hashmap_put(added_props, v->property_names[p], &was_there);
                rc = xmlTextWriterStartElement(writer, BAD_CAST "key");
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "for", BAD_CAST "node");
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST v->property_names[p]);
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "attr.name", BAD_CAST v->property_names[p]);
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
                rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "attr.type", BAD_CAST "string");
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
                rc = xmlTextWriterEndElement(writer);
                if (rc < 0) {
                    *e = WRITE_ERROR;
                    return;
                }
            }
        }
    }
    hashmap_free(added_props);
    rc = xmlTextWriterStartElement(writer, BAD_CAST "graph");
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST g->name);
    if (rc < 0) {
        *e = WRITE_ERROR;
        return;
    }
    map_t stored_edges = hashmap_new();
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
        if (v->label) {
            rc = xmlTextWriterStartElement(writer, BAD_CAST "data");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "key", BAD_CAST "d3");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterStartElement(writer, BAD_CAST "y:ShapeNode");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "y:NodeLabel", "%s", v->label);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            /*
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "alignment", BAD_CAST "center");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "fontSize", BAD_CAST "13");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "autoSizePolicy", BAD_CAST "content");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            */
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
        }
        char *attr = (char *) malloc(666 * sizeof(char));
        for (int p_index = 0; p_index < v->property_size; p_index++) {
            char *value_string = cast_value_string(v->property_types[p_index], v->property_values[p_index]);
            rc = xmlTextWriterStartElement(writer, BAD_CAST "data");
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "key", BAD_CAST v->property_names[p_index]);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteFormatString(writer, "%s", value_string);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            //char *type_string = cast_type_string(v->property_types[p_index]);
            /*
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "property_name", BAD_CAST v->property_names[p_index]);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "property_type", BAD_CAST type_string);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
            rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "property_value", BAD_CAST value_string);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
             */
            //sprintf(attr, "%s %s", v->property_names[p_index], type_string);
            //rc = xmlTextWriterWriteAttribute(writer, BAD_CAST attr, BAD_CAST value_string);
            // CRITICAL EXECUTION
            //free(type_string);
            // CRITICAL EXECUTION
            //free(value_string);
            rc = xmlTextWriterEndElement(writer);
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
            //free(edge_id);
            rc = xmlTextWriterEndElement(writer);
            if (rc < 0) {
                *e = WRITE_ERROR;
                return;
            }
        }
        free(attr);
    }
    hashmap_free(stored_edges);
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
