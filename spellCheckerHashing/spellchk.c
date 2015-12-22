// $Id: spellchk.c,v 1.6 2013-05-21 19:58:24-07 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ctype.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"
#include "strhash.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

void print_error (char *object, char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = EXIT_FAILURE;
}

FILE *open_infile (char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) 
   {
      print_error (filename, strerror (errno));
      Exit_Status = 2;
   }
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      //STUBPRINTF ("%s: %d: %s\n", filename, yylineno, yytext);
      if(has_hashset(hashset, yytext)) continue;
      else {
         printf("%s: %d: %s\n", filename, yylineno, yytext);
         if(Exit_Status == 0) Exit_Status++;
      }
   }
}

void load_dictionary (char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   //STUBPRINTF ("Open dictionary, load it, close it\n");
   char buffer[1024];
   FILE *dict = fopen(dictionary_name, "r");
   if(dict == NULL) {
      fflush(NULL);
      fprintf(stderr, "%s: %s: No such file or directory\n",
              Exec_Name, dictionary_name);
      fflush(NULL);
      Exit_Status = 2;
      return;
   }
   for(;;) {
      char *scancount = fgets(buffer, sizeof buffer, dict);
      if(scancount == NULL) break;
      char *nlpos = strchr(buffer, '\n');
      if(nlpos != NULL) {*nlpos = '\0';}
      nlpos = strdup(buffer);
      put_hashset(hashset, nlpos);
  }
  fclose(dict);
}


int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   int debug = 0;

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': //STUBPRINTF ("-x\n");
                   debug++;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   if(Exit_Status == 2) {
      user_dictionary = NULL;
   }

   // Read and do spell checking on each of the files.
   if(user_dictionary == NULL && default_dictionary == NULL) {
      fflush(NULL);
      fprintf(stderr, "%s: dictionary is empty\n", Exec_Name);
      fflush(NULL);
      return 2;
   }
  
   if(debug > 0) {
      debugcheck (hashset, debug);
      yylex_destroy();
      free_hashset(hashset);
      return Exit_Status;
   }

   
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   
   free_hashset(hashset);
   yylex_destroy ();
   return Exit_Status;
}

