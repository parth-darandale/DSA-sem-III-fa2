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

    bool isValidInfix(string infix) {
        int operandCount = 0, operatorCount = 0;
        bool lastWasOperand = false;
        int parenthesesBalance = 0;

        for (int i = 0; i < infix.length(); i++) {
            char c = infix[i];
            if (isspace(c)) {
                continue;
            }
            if (isdigit(c)) {
                operandCount++;
                lastWasOperand = true;
                while (i + 1 < infix.length() && isdigit(infix[i + 1])) {
                    i++;
                }
            } else if (isOperator(c)) {
                operatorCount++;
                if (!lastWasOperand) {
                    return false;
                }
                lastWasOperand = false;
            } else if (c == '(') {
                parenthesesBalance++;
                stack.push(string(1, c));
                lastWasOperand = false;
            } else if (c == ')') {
                parenthesesBalance--;
                if (parenthesesBalance < 0 || stack.empty()) {
                    return false;
                }
                stack.pop();
                lastWasOperand = true;
            } else {
                return false;
            }
        }
        return (parenthesesBalance == 0 && operandCount == operatorCount + 1);
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    string infixToPostfix(string infix) {
        stringstream result;
        for (char c : infix) {
            if (isspace(c)) {
                continue;
            }
            if (isalnum(c)) {
                result << c;
            } else if (c == '(') {
                stack.push(string(1, c));
            } else if (c == ')') {
                while (!stack.empty() && stack.peek() != "(") {
                    result << stack.pop();
                }
                if (!stack.empty()) {
                    stack.pop();
                }
            } else {
                while (!stack.empty() && precedence(stack.peek()[0]) >= precedence(c)) {
                    result << stack.pop();
                }
                stack.push(string(1, c));
            }
        }
        while (!stack.empty()) {
            result << stack.pop();
        }
        return result.str();
    }

    string infixToPrefix(string infix) {
        infix = reverseString(infix);
        for (int i = 0; i < infix.length(); i++) {
            if (infix[i] == '(') {
                infix[i] = ')';
            } else if (infix[i] == ')') {
                infix[i] = '(';
            }
        }
        string postfix = infixToPostfix(infix);
        return reverseString(postfix);
    }

    string postfixToInfix(string postfix) {
        Stack<string> s;
        for (char c : postfix) {
            if (isalnum(c)) {
                s.push(string(1, c));
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = "(" + operand1 + c + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string postfixToPrefix(string postfix) {
        Stack<string> s;
        for (char c : postfix) {
            if (isalnum(c)) {
                s.push(string(1, c));
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = string(1, c) + operand1 + operand2;
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToInfix(string prefix) {
        Stack<string> s;
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            if (isalnum(c)) {
                s.push(string(1, c));
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = "(" + operand1 + c + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToPostfix(string prefix) {
        Stack<string> s;
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            if (isalnum(c)) {
                s.push(string(1, c));
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = operand1 + operand2 + c;
                s.push(expr);
            }
        }
        return s.pop();
    }

    int evaluatePostfix(string postfix) {
        Stack<int> s;
        for (char c : postfix) {
            if (isdigit(c)) {
                s.push(c - '0');
            } else {
                int operand2 = s.pop();
                int operand1 = s.pop();
                switch (c) {
                    case '+':
                        s.push(operand1 + operand2);
                        break;
                    case '-':
                        s.push(operand1 - operand2);
                        break;
                    case '*':
                        s.push(operand1 * operand2);
                        break;
                    case '/':
                        s.push(operand1 / operand2);
                        break;
                }
            }
        }
        return s.pop();
    }

    int evaluatePrefix(string prefix) {
        Stack<int> s;
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            if (isdigit(c)) {
                s.push(c - '0');
            } else {
                int operand1 = s.pop();
                int operand2 = s.pop();
                switch (c) {
                    case '+':
                        s.push(operand1 + operand2);
                        break;
                    case '-':
                        s.push(operand1 - operand2);
                        break;
                    case '*':
                        s.push(operand1 * operand2);
                        break;
                    case '/':
                        s.push(operand1 / operand2);
                        break;
                }
            }
        }
        return s.pop();
    }

    int evaluateInfix(string infix) {
        string postfix = infixToPostfix(infix);
        return evaluatePostfix(postfix);
    }

    string reverseString(string expr) {
        reverse(expr.begin(), expr.end());
        return expr;
    }
};

int main() {
    Calculator calc;
    string infix, postfix, prefix;

    cout << "Enter an infix expression: ";
    getline(cin, infix);

    if (calc.isValidInfix(infix)) {
        postfix = calc.infixToPostfix(infix);
        prefix = calc.infixToPrefix(infix);

        cout << "Infix to Postfix: " << postfix << endl;
        cout << "Infix to Prefix: " << prefix << endl;

        cout << "Evaluation of Postfix: " << calc.evaluatePostfix(postfix) << endl;
        cout << "Evaluation of Prefix: " << calc.evaluatePrefix(prefix) << endl;
        cout << "Evaluation of Infix: " << calc.evaluateInfix(infix) << endl;

        string convertedInfix = calc.postfixToInfix(postfix);
        cout << "Postfix to Infix: " << convertedInfix << endl;

        string convertedPrefix = calc.postfixToPrefix(postfix);
        cout << "Postfix to Prefix: " << convertedPrefix << endl;

        string convertedPostfix = calc.prefixToPostfix(prefix);
        cout << "Prefix to Postfix: " << convertedPostfix << endl;

        string convertedInfixFromPrefix = calc.prefixToInfix(prefix);
        cout << "Prefix to Infix: " << convertedInfixFromPrefix << endl;
    } else {
        cout << "Invalid infix expression!" << endl;
    }

    return 0;
}
