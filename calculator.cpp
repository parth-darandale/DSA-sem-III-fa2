#include<iostream>
#include<string>
#include<sstream>
#include<cctype>
#include "stack.h"

using namespace std;

class Calculator{
    Stack<string> stack;

    public:
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    bool isValidPostfix(string postfix) {
        int operandCount = 0;
        
        for (char c : postfix) {
            if (isalnum(c)) {
                operandCount++;
            } else if (isOperator(c)) {
                if (operandCount < 2) {
                    return false;
                }
                operandCount--;
            } else {
                return false;
            }
        }
        
        return operandCount == 1;
    }

    bool isValidInfix(string infix) {
        int operandCount = 0, operatorCount = 0;

        for (char c : infix) {
            if (isalnum(c)) {
                operandCount++;
            } else if (isOperator(c)) {
                operatorCount++;
                if (operandCount <= operatorCount) {
                    return false;
                }
            } else if (c == '(') {
                stack.push(c);
            } else if (c == ')') {
                if (stack.empty()) {
                    return false;
                }
                stack.pop();
            } else {
                return false;
            }
        }

        if (!stack.empty()) {
            return false;
        }

        return operandCount == operatorCount + 1;
    }

    bool isValidPrefix(string prefix) {
        int operandCount = 0, operatorCount = 0;
        
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            
            if (isalnum(c)) {
                operandCount++;
            } else if (isOperator(c)) {
                operatorCount++;
                if (operandCount <= operatorCount) {
                    return false;
                }
            } else {
                return false;
            }
        }
        
        return operandCount == operatorCount + 1;
    }


    string postfixToInfix(string postfix) {
        if(!isValidPostfix(postfix)){
            cout << "Invalid postfix expression" << endl;
            return " "; 
        } 

        for (char c : postfix) {
            if (isalnum(c)) {
                stringstream ss;
                ss << c;
                stack.push(ss.str());
            } else if (isOperator(c)) {
                string operand1 = stack.pop();
                string operand2 = stack.pop();

                stringstream ss;
                ss << "(" << operand2 << c << operand1 << ")";
                stack.push(ss.str());
            }
        }
        return stack.pop();
    } 

    string    
};  