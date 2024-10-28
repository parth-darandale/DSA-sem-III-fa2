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
        stringstream result, operand;
        for (char c : infix) {
            if (isspace(c)) {
                if (!operand.str().empty()) {  // If there's an operand being built, push it to the result
                    result << operand.str() << " ";
                    operand.str("");  // Clear the operand buffer
                    operand.clear();
                }
                continue;
            }

            if (isalnum(c)) {
                operand << c;  // Collect multi-digit numbers or variables
            } else if (c == '(') {
                stack.push(string(1, c));
            } else if (c == ')') {
                if (!operand.str().empty()) {  // Add any pending operand before handling the parenthesis
                    result << operand.str() << " ";
                    operand.str("");  // Clear the operand buffer
                    operand.clear();
                }
                while (!stack.empty() && stack.peek() != "(") {
                    result << stack.pop() << " ";
                }
                if (!stack.empty()) {
                    stack.pop();  // Pop the '('
                }
            } else {  // Operator
                if (!operand.str().empty()) {
                    result << operand.str() << " ";  // Add pending operand
                    operand.str("");  // Clear the operand buffer
                    operand.clear();
                }
                while (!stack.empty() && precedence(stack.peek()[0]) >= precedence(c)) {
                    result << stack.pop() << " ";
                }
                stack.push(string(1, c));  // Push the operator
            }
        }
        if (!operand.str().empty()) {
            result << operand.str() << " ";  // Add any remaining operand
        }
        while (!stack.empty()) {
            result << stack.pop() << " ";  // Pop remaining operators
        }
        return result.str();
    }

    string infixToPrefix(string infix) {
        // Reverse the infix expression and swap parentheses
        infix = reverseString(infix);
        for (int i = 0; i < infix.length(); i++) {
            if (infix[i] == '(') infix[i] = ')';
            else if (infix[i] == ')') infix[i] = '(';
        }

        // Convert to postfix and reverse the result
        string postfix = infixToPostfix(infix);
        return reverseString(postfix);
    }

    string postfixToInfix(string postfix) {
        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);  // If it's a number or variable, push it to the stack
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = "(" + operand1 + token + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string postfixToPrefix(string postfix) {
        Stack<string> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isalnum(token[0])) {
                s.push(token);  // Push operands to the stack
            } else {
                string operand2 = s.pop();
                string operand1 = s.pop();
                string expr = token + operand1 + operand2;  // Form the prefix expression
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToInfix(string prefix) {
        Stack<string> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        // Collect all tokens in reverse order
        while (ss >> token) {
            tokens.push_back(token);
        }

        // Process in reverse
        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isalnum(tokens[i][0])) {
                s.push(tokens[i]);  // Push operands
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = "(" + operand1 + tokens[i] + operand2 + ")";
                s.push(expr);
            }
        }
        return s.pop();
    }

    string prefixToPostfix(string prefix) {
        Stack<string> s;
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        // Collect all tokens in reverse order
        while (ss >> token) {
            tokens.push_back(token);
        }

        // Process in reverse
        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isalnum(tokens[i][0])) {
                s.push(tokens[i]);  // Push operands
            } else {
                string operand1 = s.pop();
                string operand2 = s.pop();
                string expr = operand1 + operand2 + tokens[i];  // Form postfix expression
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

    bool isValidPostfix(string postfix) {
        Stack<int> s;
        for (char c : postfix) {
            if (isspace(c)) {
                continue;  // Ignore spaces
            }
            if (isalnum(c)) {
                s.push(1);  // Push a dummy value (just to indicate a valid operand)
            } else if (isOperator(c)) {
                if (s.empty()) {
                    return false; // No operands available for the operator
                }
                s.pop(); // Remove one operand
                if (s.empty()) {
                    return false; // No operand available for the second operand
                }
                s.pop(); // Remove the second operand
                s.push(1); // Push a dummy value back for the result
            } else {
                return false; // Invalid character
            }
        }
        return s.size() == 1; // There should be exactly one result in the stack
    }

    bool isValidPrefix(string prefix) {
        Stack<int> s;
        for (int i = prefix.length() - 1; i >= 0; i--) {
            char c = prefix[i];
            if (isspace(c)) {
                continue; // Ignore spaces
            }
            if (isalnum(c)) {
                s.push(1); // Push a dummy value (just to indicate a valid operand)
            } else if (isOperator(c)) {
                if (s.empty()) {
                    return false; // No operands available for the operator
                }
                s.pop(); // Remove one operand
                if (s.empty()) {
                    return false; // No operand available for the second operand
                }
                s.pop(); // Remove the second operand
                s.push(1); // Push a dummy value back for the result
            } else {
                return false; // Invalid character
            }
        }
        return s.size() == 1; // There should be exactly one result in the stack
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

    int evaluatePostfix(string postfix) {
        Stack<int> s;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            if (isdigit(token[0])) {
                s.push(stoi(token));
            } else {
                int operand2 = s.pop();
                int operand1 = s.pop();
                switch (token[0]) {
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
        stringstream ss(prefix);
        vector<string> tokens;
        string token;

        while (ss >> token) {
            tokens.push_back(token);
        }

        for (int i = tokens.size() - 1; i >= 0; i--) {
            if (isdigit(tokens[i][0])) {
                s.push(stoi(tokens[i]));
            } else {
                int operand1 = s.pop();
                int operand2 = s.pop();
                switch (tokens[i][0]) {
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

private:
    string reverseString(string str) {
        reverse(str.begin(), str.end());
        return str;
    }
};

int main() {
    Calculator calc;

    string infix = "10 + 2 * 6";
    cout << "Infix to Postfix: " << calc.infixToPostfix(infix) << endl;
    cout << "Infix to Prefix: " << calc.infixToPrefix(infix) << endl;

    string postfix = "10 2 6 * +";
    cout << "Postfix to Infix: " << calc.postfixToInfix(postfix) << endl;
    cout << "Postfix to Prefix: " << calc.postfixToPrefix(postfix) << endl;

    string prefix = "+ 10 * 2 6";
    cout << "Prefix to Infix: " << calc.prefixToInfix(prefix) << endl;
    cout << "Prefix to Postfix: " << calc.prefixToPostfix(prefix) << endl;

    cout << "Evaluate Postfix: " << calc.evaluatePostfix(postfix) << endl;
    cout << "Evaluate Prefix: " << calc.evaluatePrefix(prefix) << endl;

    return 0;
}
