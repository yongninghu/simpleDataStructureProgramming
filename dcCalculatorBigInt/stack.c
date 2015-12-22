// $Id: stack.c,v 1.5 2013-02-19 14:46:04-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

typedef struct stack_node stack_node;

static char *stack_tag = "struct stack";
static char *stack_node_tag = "struct stack_node";

struct stack {
   char *tag;
   size_t length;
   stack_node *top;
};

struct stack_node {
   char *tag;
   stack_item item;
   stack_node *link;
};

stack *new_stack (void) {
   stack *stack = malloc (sizeof (struct stack));
   assert (stack != NULL);
   stack->tag = stack_tag;
   stack->length = 0;
   stack->top = NULL;
   return stack;
}

void free_stack (stack *stack) {
   assert (is_stack (stack));
   assert (is_empty_stack (stack));
   memset (stack, 0, sizeof stack);
   free (stack);
}


void push_stack (stack *stack, stack_item item) {
   assert (is_stack (stack));
   if(stack->top == NULL) {
      stack->length++;
      stack->tag = stack_tag;
      stack_node *new = malloc (sizeof (struct stack_node));
      assert(new != NULL);
      new->item = item;
      new->link = NULL;
      stack->top = new;
   } else {
      stack->length++;
      stack_node *new = malloc (sizeof(struct stack_node));
      assert (new != NULL);
      new->tag = stack_node_tag;
      new->item = item;
      new->link = stack->top;
      stack->top = new;
   }
}

stack_item pop_stack (stack *stack) {
   assert (is_stack (stack));
   assert (! is_empty_stack (stack));
   stack_item forreturn = stack->top->item;
   stack_node *forfree = stack->top;
   if(stack->top->link != NULL) {
      stack->length--;
      stack->top = stack->top->link;
      free(forfree);
   } else {
      stack->length--;
      free(forfree);
      stack->top = NULL; 
   }
   assert(forreturn != NULL);
   return forreturn;
} 

stack_item peek_stack (stack *stack, int index) {
   assert (is_stack (stack));
   assert (index >= 0);
   assert (index < length_stack (stack));
   stack_node *curr = stack->top;
   assert (curr != NULL);
   while(curr != NULL && index >= 1) {
      curr = curr->link;
      index--;
   }
   return curr->item;   
}

bool is_empty_stack (stack *stack) {
   assert (is_stack (stack));
   return length_stack (stack) == 0;
}

int length_stack (stack *stack) {
   assert (is_stack (stack));
   return stack->length;
}

bool is_stack (stack *stack) {
   return stack != NULL && stack->tag == stack_tag;
}

