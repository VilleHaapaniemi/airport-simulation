#pragma once

const int maxqueue = 1000;
typedef Plane Queue_entry;

class Queue {
public:
   Queue();
   bool empty() const;
   int size() const;
   Error_code serve();
   Error_code append(const Queue_entry &item);
   Error_code retrieve(Queue_entry &item) const;
   //Error_code serve_and_retrieve(Queue_entry &item);

protected:
   int count;
   int front, rear;
   Queue_entry entry[maxqueue];
};