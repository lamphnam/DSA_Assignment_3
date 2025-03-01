/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Stack.h
 * Author: LTSACH
 *
 * Created on 19 August 2020, 22:01
 */

#ifndef STACK_H
#define STACK_H

#include <list>

#include "list/DLinkedList.h"
#include "stacknqueue/IDeck.h"

template <class T>
class Stack : public IDeck<T> {
public:
    class Iterator; // forward declaration

protected:
    DLinkedList<T> list;                      // internal list
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not

public:
    Stack(void (*deleteUserData)(DLinkedList<T> *) = 0, bool (*itemEqual)(T &, T &) = 0) {
        this->itemEqual = itemEqual;
        this->deleteUserData = deleteUserData;
    }

    void push(T item) {
        // TODO: add item to the top
        this->list.add(0, item);
    }

    T pop() {
        // TODO: remove and return the top item
        if(this->list.empty()) throw std::out_of_range("Stack Underflow");
        return this->list.removeAt(0);
    }

    T &peek() {
        // TODO: return the top item
        if(this->list.empty()) throw std::out_of_range("Stack Underflow");
        return this->list.get(0);
    }

    bool empty() {
        // TODO: check if the stack is empty
        return this->list.empty();
    }

    int size() {
        // TODO: return the number of items in the stack
        return this->list.size();
    }

    void clear() {
        // TODO: remove all items in the stack
        this->list.clear();
    }

    bool remove(T item) {
        // Find the first occurrence of the item and remove it
        for(int i = 0; i < this->list.size(); i++) {
            if(equals(this->list.get(i), item, this->itemEqual)) {
                this->list.removeAt(i);
                return true;
            }
        }
        return false;
    }

    bool contains(T item) {
        // Check if the item exists in the stack
        for(int i = 0; i < this->list.size(); i++) {
            if(equals(this->list.get(i), item, this->itemEqual)) {
                return true;
            }
        }
        return false;
    }

    string toString(string (*item2str)(T &) = 0) {
        stringstream os;
        os << "FROM TOP: " << list.toString(item2str);
        return os.str();
    }

    void println(string (*item2str)(T &) = 0) {
        cout << toString(item2str) << endl;
    }

    ///
    Iterator top() {
        return Iterator(this, true);
    }

    Iterator bottom() {
        return Iterator(this, false);
    }

private:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &)) {
        if(itemEqual == 0) return lhs == rhs;
        else return itemEqual(lhs, rhs);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // Iterator: BEGIN
    class Iterator {
    private:
        Stack<T> *stack;
        typename DLinkedList<T>::Iterator listIt;

    public:
        Iterator(Stack<T> *stack = 0, bool begin = true) {
            this->stack = stack;
            if(begin) {
                if(stack != 0) this->listIt = stack->list.begin();
                else this->listIt = 0;
            } else {
                if(stack != 0) this->listIt = stack->list.end();
                else this->listIt = 0;
            }
        }

        Iterator &operator=(const Iterator &iterator) {
            this->stack = iterator.stack;
            this->listIt = iterator.listIt;
            return *this;
        }

        T &operator*() {
            return *(this->listIt);
        }

        bool operator!=(const Iterator &iterator) {
            return this->listIt != iterator.listIt;
        }

        // Prefix ++ overload
        Iterator &operator++() {
            listIt++;
            return *this;
        }

        // Postfix ++ overload
        Iterator operator++(int) {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        void remove(void (*removeItem)(T) = 0) {
            listIt.remove(removeItem);
        }
    };

    // Iterator: END
};

#endif /* STACK_H */
