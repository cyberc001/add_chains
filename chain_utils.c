#include "chain_utils.h"

#include <string.h>
#include <stdio.h>

void print_chain(unsigned* chain)
{
        for(size_t i = 0; chain[i]; ++i)
                printf("%u ", chain[i]);
        puts("");
}

size_t chain_length(unsigned* chain)
{
        size_t ln = 0;
        for(; chain[ln]; ++ln)
                ;
        return ln;
}

unsigned* optimize_chain(unsigned* chain)
{
        size_t ln = chain_length(chain);
        size_t new_ln = ln;

        for(size_t i = 0; i < new_ln - 1; ++i)
                for(size_t j = i + 1; j < new_ln; ++j)
                {
                        if(chain[i] == chain[j]){
                                memmove(chain + j, chain + j + 1, (new_ln - j + 1) * sizeof(unsigned));
                                --new_ln; --i;
                        }
                }
        return realloc(chain, sizeof(unsigned) * (new_ln + 1));
}

static int sort_chain_cmp(const unsigned* u1, const unsigned* u2)
{
        if(*u1 < *u2) return -1;
        return *u1 > *u2;
}
unsigned* sort_chain(unsigned* chain)
{
        qsort(chain, chain_length(chain), sizeof(unsigned), (int(*)(const void*, const void*))sort_chain_cmp);
        return chain;
}
