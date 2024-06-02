#ifndef CENSOR_WORDS_H
#define CENSOR_WORDS_H

#include <stdlib.h> // For size_t type

char **load_censored_words(const char *filename, int *count);
void free_censored_words(char **words, int count);
void censor_words(char *text, char **censor_words, int num_censor_words);
void censor_text_from_file(char *text, const char *filename);

#endif // CENSOR_WORDS_H
