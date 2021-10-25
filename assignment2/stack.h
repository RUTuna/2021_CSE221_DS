#ifndef STACK_H
#define STACK_H

#include <iostream>

using namespace std;

template <typename type>
struct SNode { // SNode 구조체 선언
public: 
	type data;
	struct SNode<type> *link = NULL; // Null로 초기화. SNode를 link할 거니까 type
};

template <typename type>
class Stack {
public:
	SNode<type> *head;
	int len;

  // Constructor
  explicit Stack();

  // Destructor
  ~Stack();

  // Store new element
  void push(const type& item);

  // Return first element
  type& top() const;

  // Remove first element
  void pop();

  // Return true if empty, false otherwise
  bool empty() const;

  // Number of elements stored
  int size() const;
};

// Implement functions below

template <typename type>
Stack<type>::Stack() {
  len=0; // 생성할 때 길이 0으로 초기화
}

template <typename type>
Stack<type>::~Stack() {
  while(len>0) pop();
}

template <typename type>
void Stack<type>::push(const type &item) {
  SNode<type> *addSNode = new SNode<type>; // 추가할 SNode
	addSNode -> data = item;
  if(len==0) { // list가 비어있으면 head에
		head = addSNode;
	} else {
		addSNode -> link = head;
		head = addSNode;
	}
  len++;
}

template <typename type>
type& Stack<type>::top() const {
  if(len!=0) return head->data;
  // else return NULL; // 수정합시다!
}

template <typename type>
void Stack<type>::pop() {
  if(len==0) return;
  SNode<type> *cur = head;
  head = cur -> link; // head를 head 다음 SNode로 변경
  delete cur;
  len--;
}

template <typename type>
bool Stack<type>::empty() const {
  return len==0;
}

template <typename type>
int Stack<type>::size() const {
  return len;
}

#endif //STACK_H
