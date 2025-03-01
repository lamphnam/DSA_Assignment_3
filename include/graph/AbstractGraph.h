/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */

#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <sstream>
#include <string>
using namespace std;
#include <iomanip>

template <class T>
class AbstractGraph : public IGraph<T> {
public:
    class Edge;       // forward declaration
    class VertexNode; // forward declaration
    class Iterator;   // forward declaration

private:
protected:
    // Using the adjacent list technique, so need to store list of nodes
    // (nodeList)
    DLinkedList<VertexNode *> nodeList;

    // Function pointers:
    bool (*vertexEQ)(T &, T &); // to compare two vertices
    string (*vertex2str)(T &);  // to obtain string representation of vertices

    // Lay cai node vertex ra thoi khong lam gi nua
    VertexNode *getVertexNode(T &vertex) {
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while(it != nodeList.end()) {
            VertexNode *node = *it;
            if(vertexEQ(node->vertex, vertex)) return node;
            it++;
        }
        return 0;
    }

    string vertex2Str(VertexNode &node) {
        return vertex2str(node.vertex);
    }

    string edge2Str(Edge &edge) {
        stringstream os;
        os << "E(" << vertex2str(edge.from->vertex) << "," << vertex2str(edge.to->vertex) << ")";
        return os.str();
    }

public:
    AbstractGraph(bool (*vertexEQ)(T &, T &) = 0, string (*vertex2str)(T &) = 0) {

        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }

    virtual ~AbstractGraph() {}

    typedef bool (*vertexEQFunc)(T &, T &);
    typedef string (*vertex2strFunc)(T &);

    vertexEQFunc getVertexEQ() {
        return this->vertexEQ;
    }

    vertex2strFunc getVertex2Str() {
        return this->vertex2str;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////// IMPLEMENTATION of IGraph API ////////////////////
    //////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight = 0) = 0;
    virtual void disconnect(T from, T to) = 0;
    virtual void remove(T vertex) = 0;

    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex) {
        // Kiểm tra xem đỉnh đã tồn tại chưa
        for(int i = 0; i < nodeList.size(); i++) {
            if(vertexEQ(nodeList.get(i)->vertex, vertex)) {
                // Nếu đỉnh đã tồn tại thì không thêm nữa
                return;
            }
        }

        // Nếu chưa tồn tại thì mới tạo node mới và thêm vào
        VertexNode *newNode = new VertexNode(vertex, this->vertexEQ, this->vertex2str);
        nodeList.add(newNode);
    }

    virtual bool contains(T vertex) {
        // TODO
        return getVertexNode(vertex) != nullptr;
    }

    virtual float weight(T from, T to) {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        VertexNode *toNode = getVertexNode(to);
        if(fromNode == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        if(toNode == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(to));
        }
        Edge *edge = fromNode->getEdge(toNode);
        if(!edge)
            throw EdgeNotFoundException("E(" + this->getVertex2Str()(from) + "," + this->getVertex2Str()(to) + ")");
        return edge->weight;
    }

    virtual DLinkedList<T> getOutwardEdges(T from) {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        if(fromNode == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        return fromNode->getOutwardEdges();
    }

    virtual DLinkedList<T> getInwardEdges(T to) {
        // TODO
        VertexNode *toNode = getVertexNode(to);
        if(toNode == nullptr) throw VertexNotFoundException(this->getVertex2Str()(to));

        DLinkedList<T> inwardVertices;
        for(auto nodeIt = nodeList.begin(); nodeIt != nodeList.end(); ++nodeIt) {
            VertexNode *node = *nodeIt;
            for(auto edgeIt = node->adList.begin(); edgeIt != node->adList.end(); ++edgeIt) {
                Edge *edge = *edgeIt;
                if(edge->to == toNode) {
                    inwardVertices.add(node->vertex);
                }
            }
        }
        return inwardVertices;
    }

    virtual int size() {
        // TODO
        return this->nodeList.size();
    }

    virtual bool empty() {
        // TODO
        return this->nodeList.empty();
    };

    virtual void clear() {
        // TODO
        for(auto nodeIt = nodeList.begin(); nodeIt != nodeList.end(); ++nodeIt) {
            VertexNode *vertexNode = *nodeIt;

            vertexNode->adList.clear();

            delete vertexNode;
        }

        nodeList.clear();
    }

    virtual int inDegree(T vertex) {
        // TODO
        VertexNode *vertexNode = getVertexNode(vertex);
        if(vertexNode == nullptr) throw VertexNotFoundException(this->getVertex2Str()(vertex));
        int degree = 0;
        for(auto nodeIt = nodeList.begin(); nodeIt != nodeList.end(); ++nodeIt) {
            VertexNode *node = *nodeIt;
            for(auto edgeIt = node->adList.begin(); edgeIt != node->adList.end(); edgeIt++) {
                Edge *edge = *edgeIt;
                if(edge->to == vertexNode) {
                    degree++;
                }
            }
        }
        return degree;
    }

    virtual int outDegree(T vertex) {

        VertexNode *vertexNode = getVertexNode(vertex);

        if(vertexNode == nullptr) throw VertexNotFoundException(this->getVertex2Str()(vertex));

        return vertexNode->adList.size();
    }

    virtual DLinkedList<T> vertices() {
        // TODO
        DLinkedList<T> vertexList;
        for(auto nodeIt = nodeList.begin(); nodeIt != nodeList.end(); ++nodeIt) {
            VertexNode *node = *nodeIt;
            vertexList.add(node->vertex);
        }
        return vertexList;
    }

    virtual bool connected(T from, T to) {
        // TODO
        VertexNode *fromNode = getVertexNode(from);
        if(fromNode == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(from));
        }
        VertexNode *toNode = getVertexNode(to);
        if(toNode == nullptr) {
            throw VertexNotFoundException(this->getVertex2Str()(to));
        }
        for(auto it = fromNode->adList.begin(); it != fromNode->adList.end(); it++) {
            Edge *edge = *it;
            if(edge->to == toNode) {
                return true;
            }
        }
        return false;
    }

    void println() {
        cout << this->toString() << endl;
    }

    virtual string toString() {
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()) {
            VertexNode *node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;

        os << left << setw(12) << "Edges:" << endl;

        nodeIt = nodeList.begin();
        while(nodeIt != nodeList.end()) {
            VertexNode *node = *nodeIt;

            typename DLinkedList<Edge *>::Iterator edgeIt = node->adList.begin();
            while(edgeIt != node->adList.end()) {
                Edge *edge = *edgeIt;
                os << edge->toString() << endl;
                edgeIt++;
            }

            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }

    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it;
     *          //continue to process vertex here!
     *      }
     */
    Iterator begin() {
        return Iterator(this, true);
    }

    Iterator end() {
        return Iterator(this, false);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // BEGIN of VertexNode
    class VertexNode {
    private:
        template <class U>
        friend class UGraphModel; // UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge *> adList;
        friend class Edge;
        friend class AbstractGraph;

        bool (*vertexEQ)(T &, T &);
        string (*vertex2str)(T &);

    public:
        VertexNode() : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ) {}

        VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
            : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ) {
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }

        T &getVertex() {
            return vertex;
        }

        void connect(VertexNode *to, float weight = 0) {
            // TODO
            Edge *existingEdge = getEdge(to);
            if(existingEdge != nullptr) {
                existingEdge->weight = weight;
            } else {
                Edge *edge = new Edge(this, to, weight);
                adList.add(edge);
                outDegree_++;
                to->inDegree_++;
            }
        }

        DLinkedList<T> getOutwardEdges() {
            DLinkedList<T> outwardVertices;
            typename DLinkedList<Edge *>::Iterator it = adList.begin();
            while(it != adList.end()) {
                Edge *edge = *it;
                outwardVertices.add(edge->to->vertex);
                it++;
            }
            return outwardVertices;
        }

        Edge *getEdge(VertexNode *to) {
            // TODO
            for(auto it = this->adList.begin(); it != this->adList.end(); it++) {
                Edge *edge = *it;
                if(edge->to == to) {
                    return edge;
                }
            }
            return nullptr;
        }

        bool equals(VertexNode *node) {
            // TODO
            if(node == nullptr) return false;
            return vertexEQ(this->vertex, node->vertex);
        }

        void removeTo(VertexNode *to) {
            for(auto it = adList.begin(); it != adList.end(); ++it) {
                Edge *edge = *it; // Get the edge at the current iterator position.
                if(edge->to == to) {
                    // Now use removeItem to remove the edge from the adjacency list.
                    adList.removeItem(edge, nullptr);
                    this->outDegree_--;
                    to->inDegree_--;
                    break;
                }
            }
        }

        int inDegree() {
            // TODO
            return this->inDegree_;
        }

        int outDegree() {
            // TODO
            return this->outDegree_;
        }

        string toString() {
            stringstream os;
            os << "V(" << this->vertex << ", "
               << "in: " << this->inDegree_ << ", "
               << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };

    // END of VertexNode

    // BEGIN of Edge
    class Edge {
    private:
        VertexNode *from;
        VertexNode *to;
        float weight;
        friend class VertexNode;
        friend class AbstractGraph;

    public:
        Edge() {}

        Edge(VertexNode *from, VertexNode *to, float weight = 0) {
            this->from = from;
            this->to = to;
            this->weight = weight;
        }

        bool equals(Edge *edge) {
            // TODO
            if(this->from == edge->from && this->to == edge->to && edge != nullptr) {
                return true;
            }
            return false;
        }

        static bool edgeEQ(Edge *&edge1, Edge *&edge2) {
            return edge1->equals(edge2);
        }

        string toString() {
            stringstream os;
            os << "E(" << this->from->vertex << "," << this->to->vertex << "," << this->weight << ")";
            return os.str();
        }
    };

    // END of Edge

    // BEGIN of Iterator
public:
    class Iterator {
    private:
        typename DLinkedList<VertexNode *>::Iterator nodeIt;

    public:
        Iterator(AbstractGraph<T> *pGraph = 0, bool begin = true) {
            if(begin) {
                if(pGraph != 0) nodeIt = pGraph->nodeList.begin();
            } else {
                if(pGraph != 0) nodeIt = pGraph->nodeList.end();
            }
        }

        Iterator &operator=(const Iterator &iterator) {
            this->nodeIt = iterator.nodeIt;
            return *this;
        }

        T &operator*() {
            return (*nodeIt)->vertex;
        }

        bool operator!=(const Iterator &iterator) {
            return nodeIt != iterator.nodeIt;
        }

        // Prefix ++ overload
        Iterator &operator++() {
            nodeIt++;
            return *this;
        }

        // Postfix ++ overload
        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

    // END of Iterator
};

#endif /* ABSTRACTGRAPH_H */
