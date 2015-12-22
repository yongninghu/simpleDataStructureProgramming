// $Id: main.c,v 1.5 2013-02-19 14:39:01-08 - - $

#include <assert.h>
#include <ctype.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bigint.h"
#include "debugf.h"
#include "stack.h"
#include "token.h"

char *execname = NULL;

bool not_enough (stack *stack, int enough) {
   DEBUGF ('m', "stack=%p, enough=%d\n", stack, enough);
   if (length_stack (stack) < enough) {
      fprintf (stderr, "%s: stack empty\n", execname);
      return true;
   }
   return false;
}

void do_push (stack *stack, char *yytext) {
   DEBUGF ('m', "stack=%p, yytext=%p=\"%s\"\n", stack, yytext, yytext);
   bigint *bigint = new_string_bigint (yytext);
   push_stack (stack, bigint);
}

void do_binop (stack *stack, bigint_binop binop) {
   DEBUGF ('m', "stack=%p, binop=%p\n", stack, binop);
   if (not_enough (stack, 2)) return;
   bigint *right = pop_stack (stack);
   bigint *left = pop_stack (stack);
   bigint *answer = binop (left, right);
   assert(answer != NULL);
   push_stack (stack, answer);
   free_bigint (left);
   free_bigint (right);
}

void do_clear (stack *stack) {
   DEBUGF ('m', "stack=%p\n", stack);
   while (! is_empty_stack (stack)) {
      bigint *bigint = pop_stack (stack);
      free_bigint (bigint);
   }
}


void do_print (stack *stack) {
   DEBUGF ('m', "stack=%p\n", stack);
   if (not_enough (stack, 1)) return;
   print_bigint (peek_stack (stack, 0), stdout);
}

void do_print_all (stack *stack) {
   DEBUGF ('m', "stack=%p\n", stack);
   int length = length_stack (stack);
   for (int index = 0; index < length; ++index) {
      print_bigint(peek_stack (stack,index),stdout);
   }
}

void unimplemented (int oper) {
   printf ("%s: ", execname);
   if (isgraph (oper)) printf ("'%c' (0%o)", oper, oper);
                  else printf ("0%o", oper);
   printf (" unimplemented\n");
}

void scan_options (int argc, char **argv) {
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "y@:");
      if (option == EOF) break;
      switch (option) {
         case '@': set_debugflags (optarg);
                   break;
         default : printf ("%s: -%c: invalid option\n",
                           execname, optopt);
                   break;
      }
   }
}

int main (int argc, char **argv) {
   program_name = basename (argv[0]);
   scan_options (argc, argv);
   stack *stack = new_stack ();
   token *scanner = new_token (stdin);
   for (;;) {
      int token = scan_token (scanner);
      if (token == EOF) break;
      switch (token) {
         case NUMBER: do_push (stack, peek_token (scanner)); break;
         case '+': do_binop (stack, add_bigint); break;
         case '-': do_binop (stack, sub_bigint); break;
         case '*': do_binop (stack, mul_bigint); break;
         case 'c': do_clear (stack); break;
         case 'f': do_print_all (stack); break;
         case 'p': do_print (stack); break;
         default: unimplemented (token); break;
      }
   }
   free_token(scanner);
   do_clear(stack);
   free_stack(stack);
   return EXIT_SUCCESS;
}
