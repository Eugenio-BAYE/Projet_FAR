#ifndef CENSOR_WORDS_H
#define CENSOR_WORDS_H

#include <stdlib.h> // For size_t type

/* load_censored_words : Load censored words from a file
 * Parameters : const char *filename (name of the file), int *count (number of censored words)
 * Returns : char** (array of censored words)
 */
char **load_censored_words(const char *filename, int *count);

/* free_censored_words : Free the array of censored words
 * Parameters : char **words (array of censored words), int count (number of censored words)
 * Returns : Nothing
 */
void free_censored_words(char **words, int count);

/* censor_words : Censor words in a text
 * Parameters : char *text (text to censor), char **censor_words (array of censored words), int num_censor_words (number of censored words)
 * Returns : Nothing
 */
void censor_words(char *text, char **censor_words, int num_censor_words);

/* censor_text_from_file : Censor text from a file
 * Parameters : char *text (text to censor), const char *filename (name of the file)
 * Returns : Nothing
 */
void censor_text_from_file(char *text, const char *filename);

#endif // CENSOR_WORDS_H
