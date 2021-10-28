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
  // whitespace 제거
  string infix_cal = infix;
  int length = infix_cal.size(), i;
  if(length==0) return "";
  for(i=0; i<length; i++){
    if(infix_cal[i]==' ') infix_cal.erase(i,1);
  }

  Queue<string> infix_Q; // 토큰화 된 expression 저장
  Stack<string> operator_S; // postfix 변환 시 operator 저장
  string postfix = ""; // postfix 변환된 값을 저장
  operator_S.push("#"); // eos 추가

  // 토큰화
  string currStr = ""; // 현재 expression
  for(i=0; ; i++){
    if(infix_cal[i]=='\0'){ // 문자열 마지막까지 왔을 때 저장된 expression 있으면 push
      if(currStr.size()>0) infix_Q.push(currStr);
      break;
    }
    if(infix_cal[i]=='-') { // binary minus or unary minus
      if(infix_cal[i-1]>='0' || infix_cal[i-1]==')') { // '-' 전이 operand거나 ')'이면 binary minus
        if(currStr.size()>0) {
          infix_Q.push(currStr); // 저장된 expression 있으면 push
          currStr = "";
        } 
        infix_Q.push("-"); // '-' push
      } else if(infix_cal[i+1]=='('){ // unary minus, "-(" -> "-1 * ("  
        infix_Q.push("-1");
        infix_Q.push("*");
      } else currStr.push_back(infix_cal[i]); // '-' 전이 operator이면 unary minus
    } else if(infix_cal[i]>='0' || infix_cal[i]=='.') currStr.push_back(infix_cal[i]); // operand면 currStr에 추가
    else {
      if(currStr.size()>0) { // 저장된 operand 있으면 push
        infix_Q.push(currStr);
        currStr = "";
      }
      currStr.push_back(infix_cal[i]);
      infix_Q.push(currStr);
      currStr = "";
    }
  }

  // infix -> postfix
  string curr;
  while(!infix_Q.empty()){
    curr = infix_Q.front();
    if(curr==")"){ // ')'이면 '('까지 pop
      for (; operator_S.top()!="("; operator_S.pop()) postfix = postfix + " " + operator_S.top();
      operator_S.pop();
    } else if(curr.size()==1 && curr<"0") { // operator면 isp, icp 비교
      for (; isp(operator_S.top())<=icp(curr); operator_S.pop()) postfix = postfix + " " + operator_S.top();
      operator_S.push(curr);
    } else { // operand면 출력
      if(postfix.size()==0) postfix = postfix + curr; // 처음엔 whitespace 없이
      else postfix = postfix + " " + curr;
    }
    infix_Q.pop();
  }
  while(operator_S.top()!="#" && !operator_S.empty()){
    postfix = postfix + " " + operator_S.top();
    operator_S.pop();
  }
  return postfix;
}

#endif //TO_POSTFIX_H
