/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "hash/xMap.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"
// #include "stacknqueue/PriorityQueue.h"
#include "sorting/DLinkedListSE.h"

////////////////////////////////    //////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T> {
private:
public:
    DGraphModel(bool (*vertexEQ)(T &, T &), string (*vertex2Str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2Str) {}

    void connect(T from, T to, float weight = 0) {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromVertex = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toVertex = this->getVertexNode(to);
        if(fromVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        if(toVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(to));
        }

        fromVertex->connect(toVertex, weight);
    }

    void disconnect(T from, T to) {
        // Retrieve VertexNode objects for from and to nodeList
        typename AbstractGraph<T>::VertexNode *fromVertex = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toVertex = this->getVertexNode(to);

        // Check if nodeList exist
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

        // Remove the vertex itself from the nodeList list
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

    static DGraphModel<T> *create(T *nodeList, int nnodeList, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &),
                                  string (*vertex2Str)(T &)) {
        // Create a new DGraphModel instance
        DGraphModel<T> *graph = new DGraphModel<T>(vertexEQ, vertex2Str);

        // Add all nodeList to the graph
        for(int i = 0; i < nnodeList; i++) {
            graph->add(nodeList[i]);
        }

        for(int i = 0; i < nedges; i++) {
            graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
