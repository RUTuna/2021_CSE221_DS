#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;

template <typename type>
struct QNode { // QNode 구조체 선언
public: 
	type data;
	struct QNode<type> *link = NULL; // Null로 초기화. QNode를 link할 거니까 type
};

template <typename type>
class Queue {
public:
	QNode<type> *head;
  QNode<type> *tail;
	int len;

  // Constructor
  explicit Queue();

  // Destructor
  ~Queue();

  // Store new element
  void push(const type& item);

  // Return first element
  type& front() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;

};

// Implement functions below

template <typename type>
Queue<type>::Queue() {
  len=0; // 생성할 때 길이 0으로 초기화
}

template <typename type>
Queue<type>::~Queue() {
  while(len>0) pop();
}


template <typename type>
void Queue<type>::push(const type &item) {
  QNode<type> *addQNode = new QNode<type>; // 추가할 QNode
	addQNode -> data = item;
  if(len==0) { // list가 비어있으면 head, tail
		head = addQNode;
    tail = addQNode;
	} else { // tail 뒤에 연결 후 tail로
		tail -> link = addQNode;
		tail = addQNode;
	}
  len++;
}

template <typename type>
type& Queue<type>::front() const {
  if(len!=0) return head->data;
}

template <typename type>
void Queue<type>::pop() {
  if(len==0) return;
  QNode<type> *cur = head;
  head = cur -> link; // head를 head 다음 QNode로 변경
  delete cur;
  len--;
}

template <typename type>
bool Queue<type>::empty() const {
  return len==0;
}

template <typename type>
int Queue<type>::size() const {
  return len;
}

#endif //QUEUE_H
