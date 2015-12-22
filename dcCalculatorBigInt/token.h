// $Id: token.h,v 1.1 2013-02-15 19:41:09-08 - - $

#ifndef __TOKEN_H__
#define __TOKEN_H__

#include <stdbool.h>

#define NUMBER 256

typedef struct token token;

token *new_token (FILE*);

void free_token (token *);

int scan_token (token *);

char *peek_token (token *);

bool is_token (token *);

#endif

