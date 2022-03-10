#ifndef ADD_ALGOS
#define ADD_ALGOS

/* follows the formula
   k = λλ(n) - 2λλλ(n)
   n should be quite big for it to be optimal, though.
*/
unsigned find_optimal_k(unsigned n);

unsigned* brouwer(unsigned n, unsigned k);
unsigned* yao(unsigned n, unsigned k);

#endif
