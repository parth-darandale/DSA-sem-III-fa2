#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "stack.h"

using namespace std;

class Calculator {
    Stack<string> stack;

public:
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    string infixToPostfix(string infix) {
        if (!isValidInfix(infix)) {
            return "Invalid expression";
        }

        stringstream result, operand;
        for (char c : infix) {
            if (isspace(c)) {
                if (!operand.str().empty()) {
                    result << operand.str() << " ";  // add operand to result
                    operand.str("");  
                    operand.clear();
                }
                continue;
            }

            if (isalnum(c)) {
                operand << c;  // accumulate digits
            } else if (c == '(') {
                stack.push(string(1, c));  // push open parentheses
            } else if (c == ')') {
                if (!operand.str().empty()) {  
                    result << operand.str() << " ";  // add operand before ')'
                    operand.str("");  
                    operand.clear();
                }
                while (!stack.empty() && stack.peek() != "(") {
                    result << stack.pop() << " ";  // pop until '('
                }
                if (!stack.empty()) {
                    stack.pop();  // remove '('
                }
            } else {  
                if (!operand.str().empty()) {
                    result << operand.str() << " ";  
                    operand.str("");  
                    operand.clear();
                }
                while (!stack.empty() && precedence(stack.peek()[0]) >= precedence(c)) {
                    result << stack.pop() << " ";  // pop higher precedence ops
                }
                stack.push(string(1, c));  
            }
        }
        if (!operand.str().empty()) {
            result << operand.str() << " ";  
        }
        while (!stack.empty()) {
            result << stack.pop() << " ";  
        }
        return result.str();
    }

    string infixToPrefix(string infix) {
        if (!isValidInfix(infix)) {
            return "Invalid expression";
        }

        infix = reverseString(infix);  // reverse for prefix conversion
        for (int i = 0; i < infix.length(); i++) {
            if (infix[i] == '(') infix[i] = ')';
            else if (infix[i] == ')') infix[i] = '(';  // swap parentheses
        }

        string postfix = infixToPostfix(infix);
        return reverseString(postfix);  // reverse postfix for prefix
    }

    string postfixToInfix(string postfix) {
        if (!isValidPostfix(postfix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);  
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = "(" + operand1 + " " + token + " " + operand2 + ")";  // create infix format
                s.push(expr);
            }
        }
        return s.pop();
    }

    string postfixToPrefix(string postfix) {
        if (!isValidPostfix(postfix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);  
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = token + " " + operand1 + " " + operand2;  // create prefix format
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToInfix(string prefix) {
        if (!isValidPrefix(prefix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(prefix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = "(" + operand1 + " " + token + " " + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToPostfix(string prefix) {
        if (!isValidPrefix(prefix)) {
            return "Invalid expression";
        }

        Stack<string> s;
        stringstream ss(prefix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = operand1 + " " + operand2 + " " + token;
                s.push(expr);
            }
        }
        return s.pop();
    }

    bool isValidInfix(string infix) {
        int operandCount = 0, operatorCount = 0;
        bool lastWasOperand = false;
        int parenthesesBalance = 0;

        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];
            if (isspace(c)) continue;

            if (isdigit(c)) {
                operandCount++;
                lastWasOperand = true;
                while (i + 1 < infix.length() && isdigit(infix[i + 1])) i++;
            } else if (isOperator(c)) {
                operatorCount++;
                if (!lastWasOperand) return false;  // ensure valid operand before operator
                lastWasOperand = false;
            } else if (c == '(') {
                parenthesesBalance++;
                stack.push(string(1, c));
                lastWasOperand = false;
            } else if (c == ')') {
                parenthesesBalance--;
                if (parenthesesBalance < 0 || stack.empty()) return false;  // unmatched ')'
                stack.pop();
                lastWasOperand = true;
            } else return false;
        }
        return (parenthesesBalance == 0 && operandCount == operatorCount + 1);
    }

    bool isValidPostfix(string postfix) {
        Stack<int> s;
        for (char c : postfix) {
            if (isspace(c)) continue;
            if (isalnum(c)) {
                s.push(1);
            } else if (isOperator(c)) {
                if (s.size() < 2) return false;  // ensure two operands for operator
                s.pop();
                s.pop();
                s.push(1);
            } else return false;
        }
        return s.size() == 1;
    }

    bool isValidPrefix(string prefix) {
        Stack<int> s;
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            if (isspace(c)) continue;
            if (isalnum(c)) {
                s.push(1);
            } else if (isOperator(c)) {
                if (s.size() < 2) return false;  // ensure two operands for operator
                s.pop();
                s.pop();
                s.push(1);
            } else return false;
        }
        return s.size() == 1;
    }

    int evaluateInfix(string infix) {
        Stack<int> operands;
        Stack<char> operators;

        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];
            if (isspace(c)) continue;

            if (isdigit(c)) {
                int operand = 0;
                while (i < infix.length() && isdigit(infix[i])) {
                    operand = operand * 10 + (infix[i] - '0');  // build multi-digit number
                    i++;
                }
                i--;
                operands.push(operand);
            } else if (c == '(') {
                operators.push(c);
            } else if (c == ')') {
                while (!operators.empty() && operators.peek() != '(') {
                    int operand2 = operands.pop();
                    int operand1 = operands.pop();
                    char op = operators.pop();
                    int result;
                    if (op == '+') result = operand1 + operand2;
                    else if (op == '-') result = operand1 - operand2;
                    else if (op == '*') result = operand1 * operand2;
                    else result = operand1 / operand2;
                    operands.push(result);
                }
                operators.pop();
            } else {
                while (!operators.empty() && 
                    ((operators.peek() == '*' || operators.peek() == '/') || 
                    ((operators.peek() == '+' || operators.peek() == '-') && 
                    (c == '+' || c == '-')))) {
                    int operand2 = operands.pop();
                    int operand1 = operands.pop();
                    char op = operators.pop();
                    int result;
                    if (op == '+') result = operand1 + operand2;
                    else if (op == '-') result = operand1 - operand2;
                    else if (op == '*') result = operand1 * operand2;
                    else result = operand1 / operand2;
                    operands.push(result);
                }
                operators.push(c);
            }
        }

        while (!operators.empty()) {
            int operand2 = operands.pop();
            int operand1 = operands.pop();
            char op = operators.pop();
            int result;
            if (op == '+') result = operand1 + operand2;
            else if (op == '-') result = operand1 - operand2;
            else if (op == '*') result = operand1 * operand2;
            else result = operand1 / operand2;
            operands.push(result);
        }
        return operands.pop();
    }

    string reverseString(const string &str) {
        return string(str.rbegin(), str.rend());
    }
};




int main() {
    Calculator calc;

    string infix = "((10  + 2) * (34  + 4))";
    cout << "Infix to Postfix: " << calc.infixToPostfix(infix) << endl;
    cout << "Infix to Prefix: " << calc.infixToPrefix(infix) << endl;

    string postfix = calc.infixToPostfix(infix);
    string n = "11 22 33 44 + + *";
    cout << "Postfix to Infix: " << calc.postfixToInfix(n) << endl;
    cout << "Postfix to Prefix: " << calc.postfixToPrefix(postfix) << endl;

    string prefix = calc.infixToPrefix(infix);
    cout << "Prefix to Infix: " << calc.prefixToInfix(prefix) << endl;
    cout << "Prefix to Postfix: " << calc.prefixToPostfix(prefix) << endl;

    cout << "Evaluate Postfix: " << calc.evaluatePostfix(postfix) << endl;
    cout << "Evaluate Prefix: " << calc.evaluatePrefix(prefix) << endl;

    return 0;
}
