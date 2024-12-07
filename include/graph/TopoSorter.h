/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   TopoSorter.h
 * Author: ltsach
 *
 * Created on July 11, 2021, 10:21 PM
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"

template <class T>
class TopoSorter {
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    static int hashFunction(char &key, int size) {
        return key % size;
    }

    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0) {
        // TODO
        this->graph = graph;
        this->hash_code = hash_code;
    }

    DLinkedList<T> sort(int mode = 0, bool sorted = true) {
        // TODO
        return (mode == 0) ? dfsSort(sorted) : bfsSort(sorted);
    }

    DLinkedList<T> bfsSort(bool sorted = true) {
        // Calculate in-degree for each vertex
        xMap<T, int> inDegreeMap = vertex2inDegree(hashFunction);

        // Get list of vertices with zero in-degree
        DLinkedListSE<T> zeroInDegreeList;
        for(auto vertexIt = graph->begin(); vertexIt != graph->end(); ++vertexIt) {
            T vertex = *vertexIt;
            int currentInDegree = inDegreeMap.get(vertex);
            if(currentInDegree == 0) {
                zeroInDegreeList.add(vertex);
            }
        }
        if(sorted) {
            zeroInDegreeList.sort();
        }
        DLinkedList<T> topoSortedList;
        Queue<T> queue;

        for(auto it = zeroInDegreeList.begin(); it != zeroInDegreeList.end(); ++it) {
            T vertex = *it;
            queue.push(vertex);
        }

        while(!queue.empty()) {
            T currentVertex = queue.pop();

            topoSortedList.add(currentVertex);

            auto neighbors = graph->getOutwardEdges(currentVertex);

            for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
                T &neighbor = *it;

                int &inDegreeRef = inDegreeMap.get(neighbor);
                inDegreeRef--;

                if(inDegreeRef == 0) {
                    queue.push(neighbor);
                }
            }
        }

        return topoSortedList;
    }

    DLinkedList<T> dfsSort(bool sorted = true) {
        // TODO
        return DLinkedList<T>();
    }

protected:
    // Helper functions
    xMap<T, int> vertex2inDegree(int (*hash)(T &, int) = 0) {
        xMap<T, int> inDegreeMap(hash);

        for(auto vertexIt = graph->begin(); vertexIt != graph->end(); ++vertexIt) {
            T vertex = *vertexIt;
            int currentInDegree = graph->inDegree(vertex);
            inDegreeMap.put(vertex, currentInDegree);
        }

        return inDegreeMap;
    }

    xMap<T, int> vertex2outDegree(int (*hash)(T &, int) = 0) {
        // TODO
        xMap<T, int> outDegreeMap(hash);

        for(auto vertexIt = graph->begin(); vertexIt != graph->end(); ++vertexIt) {
            T vertex = *vertexIt;
            int currentOutDegree = graph->outDegree(vertex);
            outDegreeMap[vertex] = currentOutDegree;
        }
        return outDegreeMap;
    }

    DLinkedList<T> listOfZeroInDegrees() {
        DLinkedList<T> zeroInDegreeList;

        for(auto vertexIt = graph->begin(); vertexIt != graph->end(); ++vertexIt) {
            T vertex = *vertexIt;

            if(graph->inDegree(vertex) == 0) {
                zeroInDegreeList.add(vertex);
            }
        }
        return zeroInDegreeList;
    }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
