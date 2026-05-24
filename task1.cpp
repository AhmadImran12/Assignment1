#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <limits>

using namespace std;

struct ExprError {
    int code;
    string message;
};

struct Token {
    string kind;
    string value;
};

int precedence(const string &op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool isRightAssociative(const string &op) {
    return op == "^";
}

char matchingClose(char open) {
    if (open == '(') return ')';
    if (open == '[') return ']';
    if (open == '{') return '}';
    return 0;
}

bool isOpeningBracket(char c) {
    return c == '(' || c == '[' || c == '{';
}

bool isClosingBracket(char c) {
    return c == ')' || c == ']' || c == '}';
}

vector<Token> tokenize(const string &expr) {

    vector<Token> tokens;
    int i = 0;
    int n = (int)expr.size();

    while (i < n) {

        if (isspace((unsigned char)expr[i])) {
            i++;
            continue;
        }

        if (expr[i] == '-') {

            bool unary = false;

            if (tokens.empty()) unary = true;
            else {
                string prev = tokens.back().kind;
                if (prev == "op" || prev == "lparen") unary = true;
            }

            if (unary && i + 1 < n && isdigit((unsigned char)expr[i + 1])) {

                string num = "-";
                i++;

                while (i < n && isdigit((unsigned char)expr[i])) {
                    num += expr[i++];
                }

                tokens.push_back({"number", num});
                continue;
            }
        }

        if (isdigit((unsigned char)expr[i])) {

            string num;

            while (i < n && isdigit((unsigned char)expr[i])) {
                num += expr[i++];
            }

            tokens.push_back({"number", num});
            continue;
        }

        if (isalpha((unsigned char)expr[i]) || expr[i] == '_') {

            string id;

            while (i < n && (isalnum((unsigned char)expr[i]) || expr[i] == '_')) {
                id += expr[i++];
            }

            tokens.push_back({"variable", id});
            continue;
        }

        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
            tokens.push_back({"op", string(1, expr[i])});
            i++;
            continue;
        }

        if (isOpeningBracket(expr[i])) {
            tokens.push_back({"lparen", string(1, expr[i])});
            i++;
            continue;
        }

        if (isClosingBracket(expr[i])) {
            tokens.push_back({"rparen", string(1, expr[i])});
            i++;
            continue;
        }

        throw ExprError{1, string("Syntax error: unexpected character '") + expr[i] + "'"};
    }

    return tokens;
}

void validateTokenOrder(const vector<Token> &tokens) {

    if (tokens.empty()) {
        throw ExprError{1, "Syntax error: empty expression"};
    }

    if (tokens.front().kind == "op") {
        throw ExprError{1, "Syntax error: expression starts with operator"};
    }

    if (tokens.back().kind == "op") {
        throw ExprError{1, "Syntax error: expression ends with operator"};
    }

    for (int i = 0; i + 1 < (int)tokens.size(); i++) {

        bool currIsOperand = tokens[i].kind == "number" || tokens[i].kind == "variable" || tokens[i].kind == "rparen";
        bool nextIsOperand = tokens[i + 1].kind == "number" || tokens[i + 1].kind == "variable" || tokens[i + 1].kind == "lparen";
        bool currIsOp = tokens[i].kind == "op";
        bool nextIsOp = tokens[i + 1].kind == "op";

        if (currIsOperand && nextIsOperand) {
            throw ExprError{1, "Syntax error: two consecutive operands"};
        }

        if (currIsOp && nextIsOp) {
            throw ExprError{1, "Syntax error: two consecutive operators"};
        }

        if (tokens[i].kind == "lparen" && tokens[i + 1].kind == "rparen") {
            throw ExprError{1, "Syntax error: empty brackets"};
        }
    }
}

vector<Token> toPostfix(const vector<Token> &tokens) {

    vector<Token> output;
    vector<Token> opStack;

    for (const Token &t : tokens) {

        if (t.kind == "number" || t.kind == "variable") {
            output.push_back(t);
        }

        else if (t.kind == "op") {

            while (!opStack.empty() && opStack.back().kind == "op") {

                string topOp = opStack.back().value;

                bool shouldPop =
                    (!isRightAssociative(t.value) && precedence(topOp) >= precedence(t.value))
                    ||
                    (isRightAssociative(t.value) && precedence(topOp) > precedence(t.value));

                if (!shouldPop) break;

                output.push_back(opStack.back());
                opStack.pop_back();
            }

            opStack.push_back(t);
        }

        else if (t.kind == "lparen") {
            opStack.push_back(t);
        }

        else if (t.kind == "rparen") {

            bool found = false;

            while (!opStack.empty()) {

                Token top = opStack.back();
                opStack.pop_back();

                if (top.kind == "lparen") {

                    if (matchingClose(top.value[0]) != t.value[0]) {
                        throw ExprError{1, "Syntax error: mismatched brackets"};
                    }

                    found = true;
                    break;
                }

                output.push_back(top);
            }

            if (!found) {
                throw ExprError{1, "Syntax error: unmatched closing bracket"};
            }
        }
    }

    while (!opStack.empty()) {

        Token top = opStack.back();
        opStack.pop_back();

        if (top.kind == "lparen") {
            throw ExprError{1, "Syntax error: unmatched opening bracket"};
        }

        output.push_back(top);
    }

    return output;
}

vector<string> collectVariables(const vector<Token> &tokens) {

    vector<string> vars;
    map<string, bool> seen;

    for (const Token &t : tokens) {

        if (t.kind == "variable" && !seen[t.value]) {
            seen[t.value] = true;
            vars.push_back(t.value);
        }
    }

    return vars;
}

long long safeMultiply(long long a, long long b) {

    if (a == 0 || b == 0) return 0;

    if (a > numeric_limits<long long>::max() / b || a < numeric_limits<long long>::min() / b) {
        throw ExprError{2, "Runtime error: multiplication overflow"};
    }

    return a * b;
}

long long safePower(long long base, long long exponent) {

    if (exponent < 0) {
        throw ExprError{2, "Runtime error: negative exponent"};
    }

    long long result = 1;

    while (exponent > 0) {

        if (exponent % 2 == 1) {
            result = safeMultiply(result, base);
        }

        exponent /= 2;

        if (exponent > 0) {
            base = safeMultiply(base, base);
        }
    }

    return result;
}

long long evaluatePostfix(const vector<Token> &postfix, const map<string, long long> &varValues) {

    vector<long long> stack;

    for (const Token &t : postfix) {

        if (t.kind == "number") {

            try {
                long long val = stoll(t.value);
                stack.push_back(val);
            }

            catch (...) {
                throw ExprError{2, "Runtime error: invalid numeric value"};
            }
        }

        else if (t.kind == "variable") {

            if (varValues.find(t.value) == varValues.end()) {
                throw ExprError{2, "Runtime error: variable value missing"};
            }

            stack.push_back(varValues.at(t.value));
        }

        else if (t.kind == "op") {

            if (stack.size() < 2) {
                throw ExprError{1, "Syntax error: malformed expression"};
            }

            long long b = stack.back();
            stack.pop_back();

            long long a = stack.back();
            stack.pop_back();

            long long result = 0;

            if (t.value == "+") result = a + b;
            else if (t.value == "-") result = a - b;
            else if (t.value == "*") result = safeMultiply(a, b);

            else if (t.value == "/") {

                if (b == 0) {
                    throw ExprError{2, "Runtime error: division by zero"};
                }

                result = a / b;
            }

            else if (t.value == "^") {
                result = safePower(a, b);
            }

            stack.push_back(result);
        }
    }

    if (stack.size() != 1) {
        throw ExprError{3, "Logical error: invalid evaluation state"};
    }

    return stack[0];
}

string postfixToString(const vector<Token> &postfix) {

    string result;

    for (int i = 0; i < (int)postfix.size(); i++) {

        if (i > 0) result += " ";
        result += postfix[i].value;
    }

    return result;
}

int solveExpression(const string &expr) {

    try {

        vector<Token> tokens = tokenize(expr);

        validateTokenOrder(tokens);

        vector<Token> postfix = toPostfix(tokens);

        cout << postfixToString(postfix) << "\n";

        vector<string> vars = collectVariables(tokens);

        map<string, long long> varValues;

        for (const string &var : vars) {

            cerr << "Enter value for " << var << ": ";

            long long value;

            if (!(cin >> value)) {

                cin.clear();

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                throw ExprError{2, "Runtime error: invalid variable input"};
            }

            varValues[var] = value;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cerr << "Press Enter to continue...";
        cin.get();

        long long result = evaluatePostfix(postfix, varValues);

        cout << result << "\n";

        return 0;
    }

    catch (ExprError &e) {

        cerr << e.message << "\n";
        return e.code;
    }

    catch (exception &e) {

        cerr << "Runtime error: " << e.what() << "\n";
        return 2;
    }

    catch (...) {

        cerr << "Unknown fatal error\n";
        return 2;
    }
}
int main() {

    int T;
    cout<<"Enter no of expression: ";
    cin >> T;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int tc = 1; tc <= T; tc++) {

        cout<<"Enter expression :";
        string expr;

        getline(cin, expr);

        int code = solveExpression(expr);

        if (code != 0) {
            return code;
        }

        cin.clear();
    }

    return 0;
}