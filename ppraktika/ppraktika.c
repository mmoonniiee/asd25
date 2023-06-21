#define _CRT_SECURE_NO_WARNINGS
#define pi 3.14
#define e 2.71

#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum ops { none, pls, mns, mul, divop, ln, lg, logb2};

struct stack {
	int value;
	enum ops op;
	struct stack *next;
};

char *skip_whitespaces(char *str) {
	char *current = str;
	while (*current == ' ') 
		current++;
	return current;
}

int validate(struct stack *head) {
	int vc = 0, opc = 0;
	struct stack *current = head;
	while (current) {
		if (current->op == none)
			vc++;
		if (current->op != none) {
			opc++;
		}
		if (current->next != NULL) {
			current = current->next;
			break;
		}
	}
	if (current->op != none)
		return 1;
	if ((opc + 1) != vc)
		return 1;
	else
		return 0;
}

struct stack *tokenize(char *str) {
	struct stack *no = NULL;
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
			//free(no);
		}
		else {
			tail->next = new;
			tail = new;
			tail->next = NULL;
		}
	}
	return head;
};

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

//void destroy(struct stack *head) {

//}

int calculate(struct stack *head) {
	int a = 0, b = 0;
	struct stack *current = head;
	while ((current->next) != NULL) {
		a = head->value;
		current = current->next;
		if (current->op == none) {
			b = current->value;
			//head = head->next;
			current = current->next;
		}
		switch (current->op) {
			case 1: 
				current->value = addition(a, b);
				break;
			case 2: 
				current->value = substraction(a, b);
				break;
			case 3: 
				current->value = multiplication(a, b);
				break;
			case 4: 
				current->value = (int)division(a, b);
				break;
			case 5: 
				current->value = (int)loge(b);
				break;
			case 6: 
				current->value = (int)log10s(b);
				break;
			case 7: 
				current->value = (int)logb2s(b);
				break;
		}
		current->op = none;
		if (head->next == current)
			head = head->next;
	}
	a = current->value;
	current = current->next;
	//destroy 
	return a;
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