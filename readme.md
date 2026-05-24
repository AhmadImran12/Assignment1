# Expression Evaluator in C++

A robust **Infix Expression Evaluator** written in C++ that:

- Converts infix expressions to postfix notation
- Supports variables and user input
- Handles operator precedence and associativity
- Supports multiple bracket types: `()`, `[]`, `{}`
- Detects syntax/runtime/logical errors
- Evaluates expressions safely using `long long`

---

## Features

### Supported Operators

| Operator | Description |
|---|---|
| `+` | Addition |
| `-` | Subtraction |
| `*` | Multiplication |
| `/` | Division |
| `^` | Power |

---

### Supported Brackets

- Parentheses `()`
- Square brackets `[]`
- Curly braces `{}`

---

### Supported Operands

- Integer numbers
- Variables (`a`, `x`, `value1`, `_temp`, etc.)

---

## Error Handling

The program throws custom errors for:

### Syntax Errors

- Invalid characters
- Two consecutive operators
- Two consecutive operands
- Empty brackets
- Mismatched brackets
- Expression starting/ending with operator

### Runtime Errors

- Division by zero
- Negative exponents
- Invalid variable input
- Overflow in multiplication

### Logical Errors

- Invalid postfix evaluation state

---

## How It Works

### Step 1 — Tokenization

The input expression is broken into tokens such as:

- numbers
- variables
- operators
- brackets

Example:

```txt
a + 5 * (b - 2)
```

Becomes:

```txt
[a] [+] [5] [*] [(] [b] [-] [2] [)]
```

---

### Step 2 — Validation

The expression structure is checked for:

- invalid syntax
- bracket mismatches
- operator placement

---

### Step 3 — Infix to Postfix Conversion

The program uses a stack-based algorithm similar to the **Shunting Yard Algorithm**.

Example:

```txt
a + 5 * (b - 2)
```

Postfix:

```txt
a 5 b 2 - * +
```

---

### Step 4 — Variable Input

The user is asked to enter values for all variables.

Example:

```txt
Enter value for a:
Enter value for b:
```

---

### Step 5 — Postfix Evaluation

The postfix expression is evaluated using a stack.

---

# File Structure

```txt
task1.cpp
```

Main components:

- `tokenize()` → converts expression into tokens
- `validateTokenOrder()` → syntax checking
- `toPostfix()` → infix to postfix conversion
- `evaluatePostfix()` → evaluates postfix expression
- `safeMultiply()` → overflow-safe multiplication
- `safePower()` → safe exponentiation
- `solveExpression()` → manages complete workflow

---

# Compilation

Using g++:

```bash
g++ task1.cpp -o evaluator
```

Run:

```bash
./evaluator
```

Windows:

```bash
g++ task1.cpp -o evaluator.exe
evaluator.exe
```

---

# Example Run

## Input

```txt
Enter no of expression: 1
Enter expression : a + 5 * (b - 2)
```

## Variable Input

```txt
Enter value for a: 10
Enter value for b: 4
```

## Output

```txt
a 5 b 2 - * +
20
```

---

# Example Expressions

### Basic Arithmetic

```txt
5 + 3 * 2
```

### Nested Brackets

```txt
(5 + 3) * [2 + {4 - 1}]
```

### Variables

```txt
x^2 + y*5
```

### Negative Numbers

```txt
-5 + 3
```

---

# Algorithms Used

- Stack-based postfix evaluation
- Operator precedence parsing
- Shunting Yard Algorithm principles
- Fast exponentiation (binary exponentiation)

---

# Time Complexity

| Operation | Complexity |
|---|---|
| Tokenization | O(n) |
| Validation | O(n) |
| Infix → Postfix | O(n) |
| Evaluation | O(n) |

Overall:

```txt
O(n)
```

---

# Limitations

- Only integer arithmetic is supported
- No floating-point numbers
- Unary minus only works for numeric literals
- No mathematical functions (`sin`, `cos`, etc.)

---

# Future Improvements

Possible enhancements:

- Floating-point support
- Unary operators for variables
- Mathematical functions
- Better overflow detection
- Expression simplification
- GUI calculator version

---

# Author

C++ Expression Evaluator Project

Built using:

- STL vectors
- maps
- exception handling
- postfix evaluation techniques
