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
    static int hashFunc(T &key, int size = 100) {
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
        xMap<T, int> inDegreeMap = vertex2inDegree(hashFunc);
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
            queue.push(*it);
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
        xMap<T, bool> visited(hashFunc);
        DLinkedList<T> topoSortedList;
        xMap<T, bool> recursionStack(hashFunc);
        for(auto vertexIt = graph->begin(); vertexIt != graph->end(); ++vertexIt) {
            T vertex = *vertexIt;
            if(!visited.containsKey(vertex)) {
                if(!dfsVisit(vertex, visited, recursionStack, topoSortedList)) {
                    return DLinkedList<T>();
                }
            }
        }
        DLinkedList<T> reversedList;
        for(auto it = topoSortedList.bbegin(); it != topoSortedList.bend(); --it) {
            reversedList.add(*it);
        }
        return reversedList;
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

    bool dfsVisit(const T &vertex, xMap<T, bool> &visited, xMap<T, bool> &recursionStack,
                  DLinkedList<T> &topoSortedList) {
        visited.put(vertex, true);
        recursionStack.put(vertex, true);

        auto neighbors = graph->getOutwardEdges(vertex);

        for(auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            T &neighbor = *it;

            if(!visited.containsKey(neighbor)) {
                if(recursionStack.containsKey(neighbor)) {
                    return false;
                }
                if(!dfsVisit(neighbor, visited, recursionStack, topoSortedList)) {
                    return false;
                }
            }
        }
        recursionStack.remove(vertex);
        topoSortedList.add(vertex);
        return true;
    }

}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */
