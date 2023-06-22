#define _CRT_SECURE_NO_WARNINGS
#define pi 3.14
#define e 2.71

#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum ops { none, pls, mns, mul, divop, lg, ln, logb2};

struct stack {
	int value;
	enum ops op;
	struct stack *next;
};

struct valueq {
	int value;
	struct valueq *next;
};

char *skip_whitespaces(char *str) {
	char *current = str;
	while (*current == ' ') 
		current++;
	return current;
}

struct stack *tokenize(char *str) {
	struct stack *head = NULL, *tail = NULL; 
	char *current = str;
	while (*current != '\n') {
		struct stack *new = malloc(sizeof(new));
		new->op = none;
		current = skip_whitespaces(current);
		if (*current >= '0' && *current <= '9') {
			char temp[9];
			for (int i = 0; i <= 8; i++)
				temp[i] = NULL;
			for (int i = -1; *current >= '0' && *current <= '9'; current++) {
				i++;
				temp[i] = *current;
			}
			new->value = atoi(temp);
		}
		else if (*current == 'l') {
			current++;
			if (*current == 'n')
				new->op = ln;
			else if (*current == 'g')
				new->op = lg;
			else if (*current == 'o') {
				new->op = logb2;
				current++;
			}
		}
		else if (*current == '*')
			new->op = mul;
		else if (*current == '+')
			new->op = pls;
		else if (*current == '-')
			new->op = mns;
		else if (*current == '/')
			new->op = divop;
		else if (*current == 'p') {
			new->value = (int)pi;
			current++;
		}
		else if (*current == 'e')
			new->value = (int)e;
		current++;
		if (head == NULL) {
			head = new;
			tail = new;
		}
		else {
			tail->next = new;
			tail = new;
			tail->next = NULL;
		}
	}
	return head;
}

int validate(struct stack *head) {
	int vc = 0, opc = 0;
	struct stack *current = head;
	while (current) {
		if (current->op == none)
			vc++;
		if (current->op != none) {
			if (current->op < 4)
				opc += 2;
			else
				opc++;
		}
		if (current->next != NULL) {
			current = current->next;
			break;
		}
	}
	if (current->op != none)
		return 1;
	if (opc != vc)
		return 1;
	else
		return 0;
}

int addition(int a, int b) {
	return a + b;
}

int substraction(int a, int b) {
	return a - b;
}

int multiplication(int a, int b) {
	return a * b;
}

int division(int a, int b) {
	return a / b;
}

int loge(int a) {
	return (int)log(a);
}

int log10s(int a) {
	return (int)log10(a);
}

int logb2s(int a) {
	return (int)log2(a);
}

struct valueq *push(struct valueq *head, int value) {
	struct valueq *new = malloc(sizeof(new));
	new->value = value;
	new->next = head;
	head = new;
	return head;
}

struct valueq *pop(struct valueq *head) {
	struct valueq *current = head;
	head = head->next;
	free(current);
	return &head;
}

int calculate(struct stack *head) {
	struct stack *valueq = NULL, *current = head;
	while (current != NULL) {
		if (current->op == none)
			valueq = push(valueq, current->value);
		if (current->op != none) {
			if (current->op < 4 && valueq->next != NULL) {
				switch (current->op) {
				case pls:
					(valueq->next)->value = addition(valueq->value, (valueq->next)->value);
					valueq = pop(&valueq);
					break;
				case mns:
					(valueq->next)->value = substraction((valueq->next)->value, valueq->value);
					valueq = pop(&valueq);
					break;
				case mul:
					valueq->value = multiplication(valueq->value, (valueq->next)->value);
					//valueq = pop(valueq);
					break;
				case divop:
					valueq->value = (int)division((valueq->next)->value, valueq->value);
					//valueq = pop(valueq);
					break;
				}
			}
			else if (current->op > 4) {
				switch (current->op) {
				case lg:
					valueq->value = (int)log10s(valueq->value);
					break;
				case ln:
					valueq->value = (int)loge(valueq->value);
					break;
				case logb2:
					valueq->value = (int)logb2s(valueq->value);
					break;
				}
			}
		}
		else {
			current = current->next;
			valueq = push(valueq, current->value);
		}
		current = current->next;
	}
	return valueq->value;
}

int main() {
	int result;
	char str[255];
	printf("vyvedete izraz: ");
	fgets(str, sizeof(str), stdin);
	struct stack *izraz = tokenize(str);
	if (validate(izraz))
		printf("nevaliden izraz \n");
	result = calculate(izraz);
	printf("rezultata e: %d", result);
	return 0;
}