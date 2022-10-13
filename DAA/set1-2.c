/* a function to read an arithmetic expression (as given in the previous
problem) from the command line and display the computed value of the expression
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
  int top;
  int capacity;
  int* array;
} stack;

stack* stack_create(int capacity) {
  stack* st = (stack*)malloc(sizeof(stack));
  if (!st) return NULL;

  st->top = -1;
  st->capacity = capacity;
  st->array = (int*)malloc(st->capacity * sizeof(int));

  return st;
}

int stack_empty(stack* st) { return st->top == -1; }

int stack_peek(stack* st) { return st->array[st->top]; }

int stack_pop(stack* st) {
  if (!stack_empty(st)) {
    return st->array[st->top--];
  }
  return '$';
}

void stack_push(stack* st, int ch) { st->array[++st->top] = ch; }

void stack_destroy(stack* st) {
  free(st->array);
  free(st);
}

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

// helper function to convert infix expression to post-fix expression
int infix_to_prefix(char* exp) {
  int exp_len = strlen(exp);

  stack* st = stack_create(exp_len);
  if (!st) {
    return -1;
  }

  int i, k;
  for (i = 0, k = -1; exp[i]; ++i) {
    if (is_operand(exp[i])) {
      // when operand is encountered
      exp[++k] = exp[i];
    } else if (exp[i] == '(') {
      // openining paranthesis;
      stack_push(st, exp[i]);
    } else if (exp[i] == ')') {
      // closing paranthesis
      while (!stack_empty(st) && stack_peek(st) != '(') {
        exp[++k] = stack_pop(st);
      }
      if (!stack_empty(st) && stack_peek(st) != '(') {
        stack_destroy(st);
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
  stack_destroy(st);
  return 0;
}

/**
 * Required function to compute the result of the expression;
 * - First it converts to postfix expression
 * - And then it uses a stack to calculate the expression
 */
int calc_expression(char* exp) {
  int res = infix_to_prefix(exp);
  if (res == -1) {
    printf("Invalid expression\n");
    return -1;
  }

  stack* st = stack_create(strlen(exp));
  for (int i = 0; exp[i]; ++i) {
    if (is_operand(exp[i])) {
      stack_push(st, (exp[i] - '0'));
      continue;
    }

    int el1 = stack_pop(st);
    int el2 = stack_pop(st);

    switch (exp[i]) {
      case '+':
        stack_push(st, el1 + el2);
        break;
      case '-':
        stack_push(st, el2 - el1);
        break;
      case '*':
        stack_push(st, el1 * el2);
        break;
      case '/':
        stack_push(st, el2 / el1);
        break;
    }
  }

  int result = stack_pop(st);
  stack_destroy(st);

  return result;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s [expression]\n", argv[0]);
    return -1;
  }

  int result = calc_expression(argv[1]);
  printf("Result = %d\n", result);
}