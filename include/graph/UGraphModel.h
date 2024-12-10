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
        typename AbstractGraph<T>::Edge *edge = fromVertex->getEdge(toVertex);
        if(!edge) {
            throw EdgeNotFoundException("E(" + this->getVertex2Str()(from) + "," + this->getVertex2Str()(to) + ")");
        }
        fromVertex->removeTo(toVertex);
        toVertex->removeTo(fromVertex);
    }

    void remove(T vertex) {
        typename AbstractGraph<T>::VertexNode *removeVertex = this->getVertexNode(vertex);
        if(removeVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(vertex));
        }

        typename DLinkedList<typename AbstractGraph<T>::VertexNode *>::Iterator it = this->nodeList.begin();
        while(it != this->nodeList.end()) {
            typename AbstractGraph<T>::VertexNode *node = *it;
            if(node != removeVertex) {
                node->removeTo(removeVertex);
            }
            ++it;
        }

        DLinkedList<T> outEdges = removeVertex->getOutwardEdges();
        typename DLinkedList<T>::Iterator outIt = outEdges.begin();
        while(outIt != outEdges.end()) {
            T toVertex = *outIt;
            removeVertex->removeTo(this->getVertexNode(toVertex));
            ++outIt;
        }

        // Remove the vertex
        this->nodeList.removeItem(removeVertex);
        delete removeVertex;
    }

    static UGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &),
                                  string (*vertex2Str)(T &)) {
        // Create a new UGraphModel instance
        UGraphModel<T> *graph = new UGraphModel<T>(vertexEQ, vertex2Str);
        for(int i = 0; i < nvertices; i++) {
            graph->add(vertices[i]);
        }
        for(int i = 0; i < nedges; i++) {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }
        return graph;
    }
};

#endif /* UGRAPHMODEL_H */
