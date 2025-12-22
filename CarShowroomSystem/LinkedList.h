#pragma once
#include <iostream>

using namespace std;

template <typename T>
class LinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;

    LinkedList() : head(nullptr), tail(nullptr) {}

    // COPY CONSTRUCTOR (Deep Copy)
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr) {
        Node* temp = other.head;
        while (temp) {
            append(temp->data);
            temp = temp->next;
        }
    }

    // ASSIGNMENT OPERATOR
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* temp = other.head;
            while (temp) {
                append(temp->data);
                temp = temp->next;
            }
        }
        return *this;
    }

    ~LinkedList() { clear(); }

    void append(T val) {
        Node* newNode = new Node(val);
        if (!head) { head = newNode; tail = newNode; }
        else { tail->next = newNode; tail = newNode; }
    }

    bool isEmpty() const { return head == nullptr; }

    void clear() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr; tail = nullptr;
    }

    int size() {
        int count = 0;
        Node* temp = head;
        while (temp) { count++; temp = temp->next; }
        return count;
    }

    bool removeById(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            Node* toDelete = head;
            head = head->next;
            if (!head) tail = nullptr;
            delete toDelete;
            return true;
        }
        Node* current = head;
        while (current->next) {
            if (current->next->data.id == id) {
                Node* toDelete = current->next;
                current->next = toDelete->next;
                if (toDelete == tail) tail = current;
                delete toDelete;
                return true;
            }
            current = current->next;
        }
        return false;
    }
};