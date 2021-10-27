#ifndef TO_POSTFIX_H
#define TO_POSTFIX_H

#include <iostream>
#include <sstream>
#include "stack.h"
#include "queue.h"

using namespace std;

int isp(const string& op){
  if(op<="(") return 8; 
  else if (op=="+" || op=="-") return 3;
  else return 2;
}

int icp(const string& op){
  if(op=="(") return 0; 
  else if (op=="+" || op=="-") return 3;
  else return 2;
}

string to_postfix(const string& infix) {
  // 공백 제거
  string infix_cal = infix;
  int length = infix_cal.size(), i;
  if(length==0) return "";
  for(i=0; i<length; i++){
    if(infix_cal[i]==' ') infix_cal.erase(i,1);
  }

  Queue<string> infix_Q;
  Stack<string> operator_S;
  string postfix = "";
  operator_S.push("#");

  // 토큰화
  string currStr = "";
  for(i=0; ; i++){
    if(infix_cal[i]=='\0'){
      if(currStr.size()>0) infix_Q.push(currStr);
      break;
    }
    // if(infix_cal[i]==' ') continue;
    if(infix_cal[i]=='-') { // "-(" -> "-1 * ("
      if(infix_cal[i-1]>='0') { // '-' 전이 operand면 binary minus
      infix_Q.push(currStr);
        currStr = "";
        currStr.push_back(infix_cal[i]);
        infix_Q.push(currStr);
        currStr = "";
      } else if(infix_cal[i+1]=='('){      
        infix_Q.push("-1");
        infix_Q.push("*");
      } else currStr.push_back(infix_cal[i]); // '-' 전이 operator이면 unary minus
    } else if(infix_cal[i]>='0' || infix_cal[i]=='.') currStr.push_back(infix_cal[i]); // 숫자면 currStr에 추가 후 push
    else {
      if(currStr.size()>0) { // operand
        infix_Q.push(currStr);
        currStr = "";
      }
      currStr.push_back(infix_cal[i]);
      infix_Q.push(currStr);
      currStr = "";
    }
  }

  string curr;
  while(!infix_Q.empty()){
    curr = infix_Q.front();
    if(curr==")"){
      for (; operator_S.top()!="("; operator_S.pop()) postfix = postfix + " " + operator_S.top();
      operator_S.pop();
    } else if(curr.size()==1 && curr<"0") { // operator
      for (; isp(operator_S.top())<=icp(curr); operator_S.pop()) postfix = postfix + " " + operator_S.top();
      operator_S.push(curr);
    } else { // operand
      if(postfix.size()==0) postfix = postfix + curr;
      else postfix = postfix + " " + curr;
    }
    infix_Q.pop();
  }
  while(operator_S.top()!="#" && !operator_S.empty()){
    postfix = postfix + " " + operator_S.top();
    operator_S.pop();
  }
  return postfix;


  // string test = "";
  //   while (!infix_Q.empty()) {
  //     test = test + infix_Q.front();
  //       // cout << infix_Q.front() << " "; // 0 1 2 3 4 5 6 7
  //       infix_Q.pop();
  //   }
  // return test;
}

#endif //TO_POSTFIX_H
