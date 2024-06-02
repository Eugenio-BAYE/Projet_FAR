// censor_words.c
#include "censor_words.h"
#include <stdio.h>
#include <string.h>

char **load_censored_words(const char *filename, int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open the file");
        return NULL;
    }

    char **words = NULL;
    char buffer[128];
    int word_count = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        char *word = strdup(buffer);
        if (!word) {
            perror("Memory allocation failed for word");
            for (int i = 0; i < word_count; i++) free(words[i]);
            free(words);
            fclose(file);
            return NULL;
        }
        words = realloc(words, sizeof(char*) * (word_count + 1));
        if (!words) {
            perror("Memory reallocation failed for words array");
            free(word);
            for (int i = 0; i < word_count; i++) free(words[i]);
            free(words);
            fclose(file);
            return NULL;
        }
        words[word_count++] = word;
    }
    fclose(file);
    *count = word_count;
    return words;
}

void free_censored_words(char **words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

void censor_words(char *text, char **censor_words, int num_censor_words) {
    char *result = malloc(strlen(text) + 1);
    if (result == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    result[0] = '\0';

    char *rest = text;
    char *token = strtok_r(rest, " ,.-", &rest);
    while (token) {
        int censored = 0;
        for (int i = 0; i < num_censor_words; i++) {
            if (strcmp(token, censor_words[i]) == 0) {
                memset(token, '*', strlen(token));
                censored = 1;
                break;
            }
        }
        strcat(result, token);
        strcat(result, " ");
        token = strtok_r(NULL, " ,.-", &rest);
    }

    strcpy(text, result);
    free(result);
}

void censor_text_from_file(char *text, const char *filename) {
    int num_words;
    char **censored_words = load_censored_words(filename, &num_words);
    if (censored_words == NULL) {
        fprintf(stderr, "Failed to load censored words.\n");
        return;
    }

    censor_words(text, censored_words, num_words);
    free_censored_words(censored_words, num_words);
}