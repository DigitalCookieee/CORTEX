#ifndef NLP_H //Prevents the compilation
#define NLP_H //Defining the header file

#include <stddef.h>
#include "memory.h"

#define MAX_WORDS 40
#define MAX_WORD_LEN 32

//Declaring the Functions
void normalize_text(char *text);
int tokenize(const char *text,char tokens[][MAX_WORD_LEN],int max_tokens);
int calculate_score(const char *query, const Fact *fact, char *matched_words,size_t matched_size);
const Fact *find_best_match(const Memory *memory,const char *query,int *best_score,char *matched_words,size_t matched_size);



#endif