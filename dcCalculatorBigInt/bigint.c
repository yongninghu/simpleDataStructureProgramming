// $Id: bigint.c,v 1.4 2013-02-19 14:46:04-08 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bigint.h"

static char *bigint_tag = "struct bigint";

struct bigint {
   char *tag;
   bool is_negative;
   size_t length;
   size_t digits;
   char *buffer;
};

static void trim_zeros (bigint *bigint) {
   while (bigint->digits > 1) {
      size_t digitpos = bigint->digits - 1;
      if (bigint->buffer[digitpos] != 0) break;
      bigint->digits--;
   }
}

static bigint *do_add (bigint *left, bigint *right) {
   int diff = 0;
   if(left->digits == right->digits) {
      for(int i = left->digits - 1; i >= 0; i--) {
         diff = left->buffer[i] - right->buffer[i];
         if(diff != 0) {
            break;
         } else {
            diff = left->digits - right->digits;
         }
      }
   }   
   bigint *new = NULL;
   int carry = 0;
   int sum = 0;
   size_t i = 0;
   if(diff > 0) {
      new = new_bigint(left->digits + 1);
   } else {
      new = new_bigint(right->digits + 1);
   }
   for(i = 0; i < new->digits; i++) {
      sum = ((i<left->digits)?left->buffer[i]:0) + 
              ((i<right->digits)? right->buffer[i]:0) + carry;
      new->buffer[i] = sum % 10;
      carry = sum / 10;
   }
   new->buffer[i] = carry;
   assert(new != NULL);
   trim_zeros(new);
   return new;
}

static bigint *do_sub (bigint *left, bigint *right) {
   bigint *new = new_bigint(left->digits);
   int borrow = 0;
   for(size_t index = 0; index < left->digits; index++) {
      int difference = left->buffer[index] - 
                       ((index<right->digits)?right->buffer[index]:0) - 
                       borrow;
      if(difference < 0) difference += 20;
      new->buffer[index] = difference % 10;
      borrow = difference/10;
   }
   trim_zeros(new);
   return new;
}


bigint *new_bigint (size_t length) {
   bigint *bigint = malloc (sizeof (struct bigint));
   assert (bigint != NULL);
   bigint->tag = bigint_tag;
   bigint->is_negative = false;
   bigint->length = length;
   bigint->digits = length;
   bigint->buffer = calloc (length, sizeof (char));
   assert (bigint->buffer != NULL);
   return bigint;
}

bigint *new_string_bigint (char *string) {
   assert (string != NULL);
   size_t length = strlen (string);
   assert (length > 0);
   bigint *bigint = new_bigint (length);
   if (*string == '_') {
      bigint->is_negative = true;
      ++string;
      --length;
   }
   int index = 0;
   while (length-- > 0) {
      char digit = string[length] - '0';
      assert (0 <= digit && digit <= 9);
      bigint->buffer[index++] = digit;
   }
   trim_zeros (bigint);
   assert(bigint != NULL);
   return bigint;
}


void free_bigint (bigint *bigint) {
   assert (is_bigint (bigint));
   free (bigint->buffer);
   memset (bigint, 0, sizeof (bigint));
   free (bigint);
}

void print_bigint (bigint *bigint, FILE *file) {
   assert (is_bigint (bigint));
   if(bigint->is_negative == true) {
      printf("%s", "-");
   }
   for(int i = bigint->digits - 1; i > (-1); i--) {
         fprintf(file, "%d", bigint->buffer[i]);
   }
   printf("\n");
}

bigint *add_bigint (bigint *left, bigint *right) {
   assert (is_bigint (left));
   assert (is_bigint (right));
   int diff = 0;
   if(left->digits == right->digits) {
      for(int i = left->digits - 1; i >= 0; i--) {
         diff = left->buffer[i] - right->buffer[i];
         if(diff != 0) {
            break;
         }
      }
   } else {
      diff = left->digits - right->digits;
   }
   bigint *new = NULL;
   if(left->is_negative == right->is_negative) {
      new = do_add(left, right);
      new->is_negative = left->is_negative;
   } else {
      bigint *larger = (diff >= 0) ? left : right;
      bigint *smaller = (larger == left) ? right : left;

      new = do_sub(larger, smaller);
      new->is_negative = larger->is_negative;
   }
   assert(new != NULL);
   return new;
}

bigint *sub_bigint (bigint *left, bigint *right) {
   assert (is_bigint (left));
   assert (is_bigint (right));
   int diff = 0;
   if(left->digits == right->digits) {
      for(int i = left->digits - 1; i >= 0; i--) {
         diff = left->buffer[i] - right->buffer[i];
         if(diff != 0) {
            break;
         }
      }
   } else {
      diff = left->digits - right->digits;
   }      
   bigint *new = NULL;   
   if(left->is_negative != right->is_negative) {
      new = do_add(left, right);
      new->is_negative = left->is_negative;
   } else {
      bigint *larger = (diff >= 0) ? left : right;
      bigint *smaller = (larger == left) ? right : left;
      new = do_sub(larger, smaller);
      if(larger == left) {
         new->is_negative = larger->is_negative;
      } else {
         new->is_negative = !larger->is_negative;
      }
   }  
   return new;
}

bigint *mul_bigint (bigint *left, bigint *right) {
   assert (is_bigint (left));
   assert (is_bigint (right));
   bigint *new = new_bigint(left->digits + right->digits);   
   size_t i = 0;
   size_t j = 0;
   for(i = 0; i < right->digits; i++) {
      int multiplicarry = 0;
      int addcarry = 0;
      for(j = 0; j < left->digits; j++) {
         int prod = right->buffer[i] * left->buffer[j] + multiplicarry;
         int tmp = new->buffer[j + i] + prod % 10 + addcarry;
         new->buffer[j + i] = tmp % 10;
         addcarry = tmp / 10;
         multiplicarry = prod / 10;
      }
      new->buffer[j + i] = multiplicarry + addcarry;
   }
   if(left->is_negative == true || right->is_negative == true) {
      new->is_negative = true;
   } else {
      new->is_negative = false;
   }
   trim_zeros(new);
   return new;
}

bool is_bigint (bigint *bigint) {
   return bigint != NULL && bigint->tag == bigint_tag;
}

