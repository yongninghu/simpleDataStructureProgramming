// $Id: bigint.h,v 1.3 2013-02-19 12:36:45-08 - - $

#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdbool.h>

typedef struct bigint bigint;

typedef bigint *(*bigint_binop) (bigint *, bigint *);

bigint *new_bigint (size_t length);

bigint *new_string_bigint (char *string);

void free_bigint (bigint *);

void print_bigint (bigint *, FILE *);

bigint *add_bigint (bigint *left, bigint *right);

bigint *sub_bigint (bigint *left, bigint *right);

bigint *mul_bigint (bigint *left, bigint *right);

bool is_bigint (bigint *);

#endif

