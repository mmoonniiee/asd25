#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define STACK_SIZE 100

struct stack {
    int data[STACK_SIZE];
    int top;
};

void initialize(struct stack *s) {
    s->top = -1;
}

int is_empty(struct stack *s) {
    return s->top == -1;
}

int is_full(struct stack *s) {
    return s->top == STACK_SIZE - 1;
}

void push(struct stack *s, int value) {
    if (is_full(s)) {
        printf("Stack overflow.\n");
        return;
    }
    s->data[++s->top] = value;
}

int pop(struct stack *s) {
    if (is_empty(s)) {
        printf("Stack underflow.\n");
        return -1;
    }
    return s->data[s->top--];
}

int calculate(int op1, int op2, char operator) {
    switch (operator) {
    case '+':
        return op1 + op2;
    case '-':
        return op1 - op2;
    case '*':
        return op1 * op2;
    case '/':
        return op1 / op2;
    case 'l':
        return log(op1);
    case 'p':
        return 3.14159265358979323846; 
    case 'e':
        return 2.71828182845904523536;  
    default:
        printf("Invalid operator.\n");
        return 0;
    }
}

int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == 'l' || c == 'p' || c == 'e';
}

int validate(char *expression) {
    struct stack s;
    initialize(&s);

    char *token = strtok(expression, " ");
    while (token != NULL) {
        if (is_operator(token[0])) {
            if (is_empty(&s) || s.top == 0) {
                printf("Invalid expression: not enough operands.\n");
                return 0;
            }
            pop(&s);
        }
        else {
            int operand = atoi(token);
            push(&s, operand);
        }

        token = strtok(NULL, " ");
    }

    if (s.top != 0) {
        printf("Invalid expression: too many operands.\n");
        return 0;
    }

    return 1;
}

int evaluate(char *expression) {
    struct stack s;
    initialize(&s);

    char *token = strtok(expression, " ");
    while (token != NULL) {
        if (is_operator(token[0])) {
            if (is_empty(&s) || s.top == 0) {
                printf("Invalid expression: not enough operands.\n");
                return 0;
            }

            int op2 = pop(&s);
            int op1 = pop(&s);
            int result = calculate(op1, op2, token[0]);
            push(&s, result);
        }
        else {
            int operand = atoi(token);
            push(&s, operand);
        }

        token = strtok(NULL, " ");
    }

    return pop(&s);
}

int main() {
    char expression[255];
    printf("Enter a postfix expression: ");
    fgets(expression, sizeof(expression), stdin);

    expression[strcspn(expression, "\n")] = '\0';  // Remove the newline character

    if (validate(expression)) {
        int result = evaluate(expression);
        printf("Result: %d\n", result);
    }

    return 0;
}