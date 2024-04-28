#ifndef CLIENT_UTILS_H
#define CLIENT_UTILS_H

/* compareFin : Checks if the provided buffer matches the specific word "fin" followed by \n. 
 * Preconditions: the buffer should not be a NULL pointer
 * Parameters: char *buffer: A pointer to a character array
 * Returns: - 1 if the buffer contains exactly "fin\n".
 *          - 0 (int): If the buffer contains any other string.
 */
int compareFin(char * buffer);

#endif
