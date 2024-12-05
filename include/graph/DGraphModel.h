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
        fromVertex->removeTo(toVertex);
    }

    void remove(T vertex) {
        typename AbstractGraph<T>::VertexNode *removeVertex = this->getVertexNode(vertex);
        if(removeVertex == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(vertex));
        }
        for(auto &vertexNode : this->nodeList) {
            vertexNode->removeTo(removeVertex);
        }

        // Remove the vertex itself from the nodeList list
        int count = 0;
        for(auto it = this->nodeList.begin(); it != this->nodeList.end();it++) {
            if(*it == removeVertex) {
                this->nodeList.removeAt(count);
                break;
            } 
            count++;
        }

        delete removeVertex;
    }

    static DGraphModel<T> *create(T *nodeList, int nnodeList, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &),
                                  string (*vertex2Str)(T &)) {
        // Create a new DGraphModel instance
        DGraphModel<T> *graph = new DGraphModel<T>(vertexEQ, vertex2Str);

        // Add all nodeList to the graph
        for(int i = 0; i < nnodeList; i++) {
            graph->addVertex(nodeList[i]);
        }

        // Add all edges to the graph
        for(int i = 0; i < nedges; i++) {
            // Find source and destination nodeList
            typename AbstractGraph<T>::VertexNode *fromVertex = graph->getVertexNode(edges[i].source->data);
            typename AbstractGraph<T>::VertexNode *toVertex = graph->getVertexNode(edges[i].destination->data);

            // Ensure both nodeList exist
            if(fromVertex == nullptr) {
                delete graph;
                throw VertexNotFoundException(vertex2Str(edges[i].source->data));
            }
            if(toVertex == nullptr) {
                delete graph;
                throw VertexNotFoundException(vertex2Str(edges[i].destination->data));
            }

            // Create and add the edge
            Edge<T> *newEdge = new Edge<T>(fromVertex, toVertex, edges[i].weight);
            fromVertex->adList.add(newEdge);
        }

        return graph;
    }
};

#endif /* DGRAPHMODEL_H */
