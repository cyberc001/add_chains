#ifndef CHAIN_UTILS_H
#define CHAIN_UTILS_H

#include <stdlib.h>

/* Prints chain in format:
   chain length | 1 2 ...
*/
void print_chain(unsigned* chain);

/* Gets size of chain not counting terminating 0 */
size_t chain_length(unsigned* chain);

/* Gets rid of duplicate numbers in chain.
   takes O(N^2) time.
*/
unsigned* optimize_chain(unsigned* chain);

/* Sorts numbers in the chain.
   takes O(Nlog(N)) time.
*/
unsigned* sort_chain(unsigned* chain);

#endif
