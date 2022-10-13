/* a function to convert an infix arithmetic expression involving the operators
+, -, /, *, (, ) and single digit constant integer operands to postfix
expression. Assume usual precedence of operators in the order of /, *, + and -.
Parentheses ( ) are used to override the precedence of operators */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
  int top;
  int capacity;
  char* array;
} stack;

stack* stack_create(int capacity) {
  stack* st = (stack*)malloc(sizeof(stack));
  if (!st) return NULL;

  st->top = -1;
  st->capacity = capacity;
  st->array = (char*)malloc(st->capacity * sizeof(char));

  return st;
}

int stack_empty(stack* st) { return st->top == -1; }

char stack_peek(stack* st) { return st->array[st->top]; }

char stack_pop(stack* st) {
  if (!stack_empty(st)) {
    return st->array[st->top--];
  }
  return '$';
}

void stack_push(stack* st, char ch) { st->array[++st->top] = ch; }

int is_operand(char op) {
  if (op >= '0' && op <= '9') return 1;
  return 0;
}

int precendence(char op) {
  switch (op) {
    case '+':
      return 1;
    case '-':
      return 1;
    case '*':
      return 2;
    case '/':
      return 2;
    default:
      return -1;
  }
}

int infix_to_prefix(char* exp) {
  int exp_len = strlen(exp);

  stack* st = stack_create(exp_len);
  if (!st) {
    return -1;
  }

  int i, k;
  for (i = 0, k = -1; exp[i]; ++i) {
    if (is_operand(exp[i])) {  // when operand is encountered
      exp[++k] = exp[i];
    } else if (exp[i] == '(') {  // openining paranthesis;
      stack_push(st, exp[i]);
    } else if (exp[i] == ')') {  // closing paranthesis
      while (!stack_empty(st) && stack_peek(st) != '(') {
        exp[++k] = stack_pop(st);
      }
      if (!stack_empty(st) && stack_peek(st) != '(') {
        return -1;
      } else {
        stack_pop(st);
      }

      printf("exp is %s, and k is %d\n", exp, k);
    } else {  // when an operator is encountered
      while (!stack_empty(st) &&
             precendence(exp[i]) <= precendence(stack_peek(st))) {
        exp[++k] = stack_pop(st);
      }
      stack_push(st, exp[i]);
    }
  }
  while (!stack_empty(st)) {
    exp[++k] = stack_pop(st);
  }
  exp[++k] = '\0';
  return 0;
}

int main() {
  // Read the expression from STDIN
  printf("Enter your expression: ");

  char exp[1000];
  scanf("%s", &exp);

  printf("The infix expression is: %s\n", exp);

  int result = infix_to_prefix(exp);
  if (result == -1) {
    printf("Invalid expression provided\n");
  } else {
    printf("The postfix expression is %s\n", exp);
  }
}
