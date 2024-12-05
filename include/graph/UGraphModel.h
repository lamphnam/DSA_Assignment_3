/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

// #include "stacknqueue/PriorityQueue.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T> {
private:
public:
    class UGraphAlgorithm;
    friend class UGraphAlgorithm;

    UGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str) {}

    void connect(T from, T to, float weight = 0) {
        // Find the vertex nodes for 'from' and 'to'
        typename AbstractGraph<T>::VertexNode *fromVertex = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toVertex = this->getVertexNode(to);
        // Check if vertices exist
        if(fromVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        if(toVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(to));
        }

        // Create edges in both directions for undirected grap
        fromVertex->connect(toVertex, weight);
        toVertex->connect(fromVertex, weight);
    }

    void disconnect(T from, T to) {
        // Find the vertex nodes for 'from' and 'to'
        typename AbstractGraph<T>::VertexNode *fromVertex = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toVertex = this->getVertexNode(to);

        // Check if vertices exist
        if(fromVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        if(toVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(to));
        }
        fromVertex->removeTo(toVertex);
        toVertex->removeTo(fromVertex);
    }

    void remove(T vertex) {
        typename AbstractGraph<T>::VertexNode *removeVertex = this->getVertexNode(vertex);
        if(removeVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(vertex));
        }

        for(auto &vertexNode : this->nodeList) {
            vertexNode->removeTo(removeVertex);
        }

        int count = 0;
        for(auto it = this->nodeList.begin(); it != this->nodeList.end(); it++) {
            if(*it == removeVertex) {
                this->nodeList.removeAt(count);
                break;
            }
            count++;
        }

        delete removeVertex;
    }

    static UGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &),
                                  string (*vertex2Str)(T &)) {
        // Create a new UGraphModel instance
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2Str);

        // Add all vertices to the graph
        for(int i = 0; i < nvertices; i++) {
            graph->addVertex(vertices[i]);
        }

        // Add all edges to the graph
        for(int i = 0; i < nedges; i++) {
            // Find source and destination vertices
            typename AbstractGraph<T>::VertexNode *fromVertex = graph->getVertexNode(edges[i].source->data);
            typename AbstractGraph<T>::VertexNode *toVertex = graph->getVertexNode(edges[i].destination->data);

            // Ensure both vertices exist
            if(fromVertex == nullptr) {
                delete graph;
                throw VertexNotFoundException(vertex2Str(edges[i].source->data));
            }
            if(toVertex == nullptr) {
                delete graph;
                throw VertexNotFoundException(vertex2Str(edges[i].destination->data));
            }

            // Create and add edges in both directions
            Edge<T> *edgeFromTo = new Edge<T>(fromVertex, toVertex, edges[i].weight);
            Edge<T> *edgeToFrom = new Edge<T>(toVertex, fromVertex, edges[i].weight);

            fromVertex->adList.add(edgeFromTo);
            toVertex->adList.add(edgeToFrom);
        }

        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
