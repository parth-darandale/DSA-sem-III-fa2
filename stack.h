/*This program is a header file containing stack
Name- Parth Darandale
Class- CS div A
PRN no.- 123B1B040*/

#include<iostream>

using namespace std;

template <typename T>

class Node{
    private:
    T data;
    Node * next;
    Node * prev;

    public:
    Node(T v){
        data = v;
        next = NULL;
        prev = NULL;
    }

    template <typename U>
    friend class Stack;
};

template <typename T>

class Stack {
    private:
        Node<T> *tail;

    public:
        Stack() {
            tail = NULL;
        }

        void push(T end) {
            Node<T> *nn = new Node<T>(end);
            if (tail == NULL) {
                tail = nn;
            } 
            
            else {
                nn->prev = tail;
                tail->next = nn;
                tail = nn;
            }
        }

        T pop(){
            if (tail == NULL){
                cout<< "Empty Stack" << endl;
                return 0;
            }


            T n = tail->data;

            Node<T> * temp = tail;
            if (!(tail->prev)){
                tail = NULL;
            }
            else{
                tail = tail->prev;
                tail->next = NULL;
            }

            delete temp;

            return n;
        }

        T peek(){
            if (tail == NULL){
                cout<< "Empty Stack" << endl;
                return 0;
            }

            return tail->data;
        }

        void display(){
            Node<T> *temp = tail;
            while(temp){
                cout << temp->data << " ";
                temp = temp->prev;

            }

            return;
        }
};