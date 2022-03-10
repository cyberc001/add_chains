#include "bin_utils.h"

unsigned upow(unsigned x, unsigned n)
{ // binary algorithm
        unsigned N = n, Y = 1, Z = x;
        while(1){
                if(N % 2 == 0)
                        N = N / 2;
                else{
                        N = N / 2;
                        Y = Z * Y;
                }
                if(N == 0)
                        break;
                Z *= Z;
        }
        return Y;
}

unsigned bin_length(unsigned x)
{
        unsigned bl = 0;
        while(x)
        { x >>= 1; ++bl; }
        return bl;
}
