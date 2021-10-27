#include <iostream>
#include "stack.h"
#include "queue.h"
#include "tree.h"
#include "to_postfix.h"

int main() {
    // part 1: stack & queue
    Stack<int> s;
    for (int i = 0; i < 8; ++i) {
        s.push(i);
    }
    while (!s.empty()) {
        cout << s.top() << " "; // 7 6 5 4 3 2 1 0
        s.pop();
    }
    cout << endl;

    Queue<int> q;
    for (int i = 0; i < 8; ++i) {
        q.push(i);
    }
    while (!q.empty()) {
        cout << q.front() << " "; // 0 1 2 3 4 5 6 7
        q.pop();
    }
    cout << endl;

    // part 2: convert infix to postfix

    cout<<"TEST 1"<<endl;
    string infix = "-1 * (2 + -3 * 4) / -2";
    string postfix = to_postfix(infix);
    cout << postfix << endl; // -1 2 -3 4 * + * -2 /
    if(postfix == "-1 2 -3 4 * + * -2 /") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    Tree* postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -5) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;

   
    cout<<"TEST 2"<<endl; 
    infix = "-(2+3)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // -1 2 3 + *
    if(postfix == "-1 2 3 + *") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -5) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;


    cout<<"TEST 3"<<endl;
    infix = "(7+4)*2-1";
    postfix = to_postfix(infix);
    cout << postfix << endl; // 7 4 + 2 * 1 –
    if(postfix == "7 4 + 2 * 1 -") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == 21) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;


    cout<<"TEST 4"<<endl;
    infix = "-10 + 3 - 2";
    postfix = to_postfix(infix);
    cout << postfix << endl; // –10 3 + 2 –
    if(postfix == "-10 3 + 2 -") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -9) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;


    cout<<"TEST 5"<<endl;
    infix = "-(5 * 2)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // -1 5 2 * *
    if(postfix == "-1 5 2 * *") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -10) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;


    cout<<"TEST 6"<<endl;
    infix = "(-2)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // -2
    if(postfix == "-2") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -2) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;


    cout<<"TEST 7"<<endl;
    infix = "-(2)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // -1 2 *
    if(postfix == "-1 2 *") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -2) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;

    
    cout<<"TEST 8"<<endl;
    infix = "5/(-(2+3))";
    postfix = to_postfix(infix);
    cout << postfix << endl; // 5 -1 2 3 + * /
    if(postfix == "5 -1 2 3 + * /") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -1) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;

    
    cout<<"TEST 9"<<endl;
    infix = "-(5 * 2)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // -1 5 2 * *
    if(postfix == "-1 5 2 * *") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -10) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;

    
    cout<<"TEST 10"<<endl;
    infix = "2-(4)";
    postfix = to_postfix(infix);
    cout << postfix << endl; // 2 4 -
    if(postfix == "2 4 -") cout<<"PASS"<<endl;
    else cout<<"FAIL"<<endl;
    
    postfix_tree = build_expression_tree(postfix);
    if(postfix_tree->eval() == -2) cout<<"Eval PASS"<<endl;
    else cout<<"Eval FAIL"<<endl;



    // part 3: evaluate an expression tree
    // Tree* postfix_tree = build_expression_tree(postfix);
    // postfix_tree->print(); // (/,*,-2,-1,+,null,null,null,null,2,*,null,null,-3,4)

    // cout << postfix_tree->eval() << endl; // -5

    // delete postfix_tree;
    return 0;
}
