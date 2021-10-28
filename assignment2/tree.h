#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <sstream>
#include <cstdlib>
#include "stack.h"
#include "queue.h"

using namespace std;

/*
 * Recursive binary tree structure for building expression tree
 *
 * Constructors: Tree(char, Tree*, Tree*) and Tree(string, Tree*, Tree*)
 *      pointers will be automatically set to NULL if omitted
 *
 * Destructor: recursively destruct whole tree
 *
 */

struct Tree {
	string expr;
	Tree* left;
	Tree* right;

	explicit Tree(char h, Tree* l = NULL, Tree* r = NULL) {
		expr = string(1, h);
		left = l;
		right = r;
	}

	explicit Tree(const string& h = string(), Tree* l = NULL, Tree* r = NULL) {
		expr = h;
		left = l;
		right = r;
	}

	~Tree() {
		delete left;
		delete right;
	}

	int size() {
		int size = 1;
		if (left != NULL) { size += left->size(); }
		if (right != NULL) { size += right->size(); }
		return size;
	}

	int len() { // null 제외한 길이
		int len = 1;
		if(expr!="null"){
			if (left != NULL) { len += left->len(); }
			if (right != NULL) { len += right->len(); }
		} else len=0;
		return len;
	}

	void print();

	double eval();
};

// -------- implement below --------
double cal(const string& op, double a, double b) { // evaluation
	if(op=="+") return a+b;
	if(op=="-") return a-b;
	if(op=="*") return a*b;
	if(op=="/") return a/b;
}


void Tree::print() {
	Queue<Tree*> level_Q;
	Tree* currNode = new Tree;
	currNode->expr = expr;
	currNode->left = left;
	currNode->right = right;
	string res = "(";
	int i=0;
	while(currNode && i<len()){
		res = res + currNode->expr + ",";
		if(currNode->left) level_Q.push(currNode->left);
		if(currNode->right) level_Q.push(currNode->right);
		if(currNode->expr!="null") i++; // null이 아닐 때만 index 증가
		if(level_Q.empty()) break;
		currNode = level_Q.front();
		level_Q.pop();
	}
	if(res[res.size()-1]==',') res.erase(res.size()-1,1); // 마지막에 , 있으면 제거
	res = res + ")";
	cout<<res<<endl;
}

double Tree::eval() {
	double evalResult;
	double lEval;
	double rEval;
	if( expr.size()==0 || expr=="null") return 0.0;
	else if(left->expr == "null" && right->expr == "null") { // operand
		return atof(expr.c_str()); // sting to double
	} else {
		lEval = left->eval();
		rEval = right->eval();
		evalResult = cal(expr, lEval, rEval);
		return evalResult;
	}
}

Tree* build_expression_tree(const string& postfix) {
	Stack<Tree*> tree_S; // expression subtree 저장
	Tree* exprTree = new Tree; // expression tree
	string expression = ""; // 토큰화된 expression
	int i=0;

	while(true){
		Tree* currTree = new Tree; // 추가할 트리의 주소 매번 생성
		while(postfix[i]!=' ' && postfix[i]!='\0'){ // 공백 단위로 토큰화
			expression.push_back(postfix[i]);
			i++;
		}
		if(expression.size() == 0) break;
		if(expression.size()==1 && expression<"0") { // operator면 2개 pop해서 subtree 제작
			currTree->right = tree_S.top();
			tree_S.pop();
			currTree->left = tree_S.top();
			tree_S.pop();
		} else { // operand면 left, right이 null인 subtree 제작
			currTree->right = currTree->left = new Tree("null");
		}
		currTree->expr = expression;
		tree_S.push(currTree);

		if(postfix[i]=='\0') break;
		expression="";
		i++;
	}
	exprTree = tree_S.top();
	return exprTree;
}

#endif //TREE_H
