// CSE221 Assignment 1

#ifndef LinkedList_h
#define LinkedList_h

#include <typeinfo>
#include <iostream>

using namespace std;

template <typename Type>
struct Node { // Node 구조체 선언
public: 
	Type data;
	struct Node<Type> *link = NULL; // Null로 초기화. Node를 link할 거니까 Type
};

template <typename Type>
class LinkedList 
{
public:
	Node<Type> *head;
	int len;

	// Constructor
	LinkedList();

	// Destructor
	~LinkedList();

	// Get the value located at index
	Type Get(const int index);

	// Add val at head
	void AddAtHead(const Type& val);

	// Add val at index
	void AddAtIndex(const int index, const Type& val);

	// Delete an element located at index
	void DeleteAtIndex(const int index);

	// Delete val in linked list
	void DeleteValue(const Type& val);

	// Move the first element of val to head
	void MoveToHead(const Type& val);

	// Rotate the linked list right by steps times
	void Rotate(const int steps);

	// Reduce value that repeats multiple times
	void Reduce();

	// Reverse at every k number of nodes at a time
	void K_Reverse(const int k);

	// str에서부터 end까지의 node를 원하는 순서(increase/decrease)로 insertSort
	void IndexSort(const int str, const int end, const bool increase);

	// Sort even and odd numbers separately then append
	void EvenOddSeparateSort();

	// Return the number of elements in the linked list
	int Size();

	// Delete all elements from the linked list
	void CleanUp();
	
	// Print the list
	void Print();

};

/*
 *	Implementation
 */

template <typename Type>
LinkedList<Type>::LinkedList()
{
	len=0; // 생성할 때 길이 0으 초기화
}

template <typename Type>
LinkedList<Type>::~LinkedList()
{
	CleanUp();
}

template <typename Type>
Type LinkedList<Type>::Get(const int index)
{
	Node<Type> *cur = head; // 현재 node
	if(index<0 || index>=len) return -1; 
	else {
		for(int i=0; i<index; i++) cur = cur -> link; // index 만큼 이동
		return cur -> data;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtHead(const Type& val)
{
	Node<Type> *addNode = new Node<Type>; // 추가할 node
	addNode -> data = val;
	len++;
	if(head == NULL) { // list가 비어있으면 head에
		head = addNode;
	} else {
		addNode -> link = head;
		head = addNode;
	}
}

template <typename Type>
void LinkedList<Type>::AddAtIndex(const int index, const Type& val)
{
	if(index<0 || index>len) return; // 유효하지 않은 범위면 return
	else if(index==0) AddAtHead(val); // 0이면 head
	else {
		Node<Type> *addNode = new Node<Type>; // 붙일 node
		Node<Type> *cur = head; // 붙일 위치의 node
		Node<Type> *prev; // 붙일 위치 바로 앞 node
		addNode -> data = val;
		for(int i=0; i<index; i++) {
			prev = cur;
			cur = cur -> link;
		}
		prev -> link = addNode;
		addNode -> link = cur;
		len++;
	}
}

template <typename Type>
void LinkedList<Type>::DeleteAtIndex(const int index)
{
	if(index<0 || index>len) return; // 유효하지 않은 범위면 return
	else if(index==0){
		Node<Type> *cur = head;
		head = cur -> link; // head를 head 다음 node로 변경
		delete cur;
		len--;
	} else {
		Node<Type> *cur = head;
		Node<Type> *prev;
		for(int i=0; i<index; i++) { // index만큼 이동
			prev = cur;
			cur = cur -> link;
		}
		prev -> link = cur -> link;
		delete cur;
		len--;
	}
}

template <typename Type>
void LinkedList<Type>::DeleteValue(const Type& val)
{
	Node<Type> *cur = head;
	Node<Type> *prev;
	for(int i=0; i<len; i++) { // len까지 이동하며 해당 값 있으면 해당 index 위치 삭제
		if(cur->data==val) DeleteAtIndex(i);
		prev = cur;
		cur = cur -> link;
	}
}

template <typename Type>
void LinkedList<Type>::MoveToHead(const Type& val)
{
	if(head->data == val) return; // 이미 head에 위치하면 그대로 반환
	Node<Type> *cur = head;
	Node<Type> *prev;
	while(cur!=NULL) { // len까지 이동하며 해당 값 있으면 head로 이동
		if(cur->data==val){
			prev -> link = cur -> link;
			cur -> link = head;
			head = cur;
			return;
		}
		prev = cur;
		cur = cur -> link;
	}
}

template <typename Type>
void LinkedList<Type>::Rotate(const int steps)
{
	if(steps<=0) return; // 음수거나 0이면 return
	Node<Type> *cur;
	Node<Type> *prev;
	for(int i=0; i<steps; i++){ // step만큼 rotate
		cur = head;
		for(int j=0; j<len-1; j++) { // cur를 tail node로
			prev = cur;
			cur = cur -> link;
		}
		prev -> link = NULL; // tail 바로 앞 node를 tail node로
		cur -> link = head; 
		head = cur; // tail을 head로
	}
}

template <typename Type>
void LinkedList<Type>::Reduce()
{
	Node<Type> *pivot=head; // head부터 비교할 node 지정
	Node<Type> *cur;
	Node<Type> *prev;
	int pivotVal; // 비교할 node 값
	for(int i=0; i<len-1; i++){
		pivotVal = pivot->data; // 비교할 값 지정
		cur=pivot->link; // pivot 다음 node부터 탐색
		for(int j=i+1; j<len; j++){
			if(cur->data==pivotVal) { // pivotVal와 같은 것 있으면 delete
				DeleteAtIndex(j);
				j--; // 하나 제거되었으니 index 한 칸 앞으로
			}
			prev = cur;
			cur = cur -> link;
		}
		pivot = pivot -> link; // pivot 한 칸씩 이동
	}
}

template <typename Type>
void LinkedList<Type>::K_Reverse(const int k)
{
	if(k<2 || k>len) return;
	int times=len/k; // reverse할 구간 개수 (정수)
	Node<Type> *cur=head;
	Node<Type> *prev;
	Node<Type> *next;
	Node<Type> *start; // reverse 시작할 node
	AddAtHead(0); // 임시값 추가
	Node<Type> *end = head; // 직전 reverse 구간의 마지막 node, 임시값 node로 초기화
	for(int i=0; i<times; i++){ // times 만큼 reverse
		start=cur;
		for(int j=0; j<k; j++){ // k개씩 reverse
			next = cur -> link;
			cur -> link = prev;
			prev = cur;
			cur = next;
		}
		end->link = prev;
		start->link = cur;
		prev = start;
		end = start;
	}
	DeleteAtIndex(0); // 임시값 삭제
}

template <typename Type>
void LinkedList<Type>::IndexSort(const int str, const int end, const bool increase) {
	Node<Type> *jNode=head->link;
	Node<Type> *iNode=head;
	int iVal, jVal, tem, temIndex;
	for(int i=0; i<str; i++){ // str node로 이동
		iNode = jNode;
		jNode = jNode -> link;
	}
	for(int i=str; i<end-1; i++){ // str부터 end까지 정렬
		iVal = iNode->data;
		jNode = iNode->link;
		iNode = iNode->link;
		tem = iVal; // 최소(최대) 값
		temIndex = i; // 최소(최대) 값의 index
		for(int j=i+1; j<end; j++){ // 최소(최대) 값 탐색
			jVal = jNode->data;
			jNode = jNode -> link;
			if((jVal<tem && increase) || (jVal>tem && !increase)) {
				temIndex=j;
				tem = jVal;
			}
		}
		// 최소(최대) 값이 i번째 값이 아니라면 i번째 값과 교환, 하나 추가해서 index 밀렸으니 +1
		if(i!=temIndex){
			AddAtIndex(i,tem);
			DeleteAtIndex(temIndex+1);
			AddAtIndex(temIndex+1,iVal);
			DeleteAtIndex(i+1);
		}
	}
}

template <typename Type>
void LinkedList<Type>::EvenOddSeparateSort() 
{
	Node<Type> *cur=head;
	Node<Type> *prev;
	int evenCount = 0; // sort 범위 지정을 위해 짝수 개수 count
	int times = 0;
	while(times<len){ // 홀수 짝수 분리
		if(cur->data%2==0){ // 짝수는 앞으로
			AddAtHead(cur->data);
			DeleteAtIndex(evenCount+1);
			evenCount++;
		} else { // 홀수는 뒤로
			AddAtIndex(len, cur->data);
			DeleteAtIndex(evenCount);
		}
		prev = cur;
		cur = cur->link;
		times++;
	}
	IndexSort(0, evenCount, true);
	IndexSort(evenCount, len, false);
}

template <typename Type>
int LinkedList<Type>::Size()
{
	return len;
}

template <typename Type>
void LinkedList<Type>::CleanUp()
{
	while(len!=0) DeleteAtIndex(0);
}

template <typename Type>
void LinkedList<Type>::Print()
{
	Node<Type> *cur = head;
	cout<<"("<<cur -> data;
	for(int i=1; i<len; i++) {
		cur = cur -> link;
		cout<< ","<<cur -> data;
	}
	cout<<")"<<endl;
}


#endif